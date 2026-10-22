class EditorSplineGenerator
{
	protected EditorSplineData m_Spline;
	protected EditorSplineGeneratorSettings m_Settings;
	protected EditorSplinePath m_Path;
	protected EditorSplineGeneratorResult m_Result;
	protected bool m_ConsoleOnly;

	static EditorSplineGeneratorResult Run(EditorSplineData spline, EditorSplinePath path, bool console_only)
	{
		EditorSplineGeneratorResult result = new EditorSplineGeneratorResult();
		if (!spline || !path) {
			result.Message = "No spline selected.";
			return result;
		}

		if (spline.Count() < 2) {
			result.Message = "Add at least two points (Ctrl + Click in the world).";
			return result;
		}

		EditorSplineGenerator generator = Create(spline.Generator.Type);
		if (!generator) {
			result.Message = "Choose a generator for this spline.";
			return result;
		}

		generator.m_Spline = spline;
		generator.m_Settings = spline.Generator;
		generator.m_Path = path;
		generator.m_Result = result;
		generator.m_ConsoleOnly = console_only;
		generator.Generate();
		return result;
	}

	static EditorSplineGenerator Create(EditorSplineGeneratorType type)
	{
		switch (type) {
			case EditorSplineGeneratorType.ASSET: return new EditorSplineAssetGenerator();
			case EditorSplineGeneratorType.ROAD: return new EditorSplineRoadGenerator();
			case EditorSplineGeneratorType.WALL: return new EditorSplineWallGenerator();
			case EditorSplineGeneratorType.FOREST: return new EditorSplineForestGenerator();
			case EditorSplineGeneratorType.FOOTPATH: return new EditorSplineFootpathGenerator();
		}

		return null;
	}

	protected void Generate();

	// Resolves asset names, counting the ones that are missing or not console friendly
	protected void ResolveAssets(array<string> names, array<EditorSplineAssetInfo> asset_infos)
	{
		foreach (string name: names) {
			EditorSplineAssetInfo asset_info = EditorSplineAssetCatalog.Get(name);
			if (!asset_info || !asset_info.Valid) {
				m_Result.Missing++;
				continue;
			}

			if (m_ConsoleOnly && !asset_info.ConsoleFriendly) {
				m_Result.NotConsoleFriendly++;
				continue;
			}

			asset_infos.Insert(asset_info);
		}
	}

	// Single asset (walls, posts), null when missing or not console friendly
	protected EditorSplineAssetInfo ResolveAsset(string name)
	{
		if (name == string.Empty) {
			return null;
		}

		EditorSplineAssetInfo asset_info = EditorSplineAssetCatalog.Get(name);
		if (!asset_info || !asset_info.Valid) {
			m_Result.Missing++;
			return null;
		}

		if (m_ConsoleOnly && !asset_info.ConsoleFriendly) {
			m_Result.NotConsoleFriendly++;
			return null;
		}

		return asset_info;
	}

	// Ground height under a point, or its own height when snapping is off
	protected float GetGroundHeight(vector position)
	{
		if (m_Settings.SnapToTerrain) {
			return EditorSplineMath.TerrainY(position);
		}

		return position[1];
	}

	// Bottom of the bounding box on ground, facing forward and standing on up. lift moves it further along up
	protected void PlaceAsset(EditorSplineAssetInfo asset_info, vector ground, vector forward, vector up, float scale, float lift, LinearColor color, float marker_height)
	{
		vector up_axis = up.Normalized();
		if (up_axis.LengthSq() < 0.0001) {
			up_axis = vector.Up;
		}

		vector transform[4];
		EditorSplineMath.ScaledFrame(forward, up_axis, ground + up_axis * (lift - asset_info.ClipMin[1] * scale), scale, transform);
		m_Result.Add(asset_info.SpawnType, transform, ground, transform[2].Normalized(), color, marker_height);
	}

	protected bool IsCapped()
	{
		if (m_Result.IsFull()) {
			m_Result.Capped = true;
			return true;
		}

		return false;
	}

	// Built the same way the editor builds data for objects placed by hand
	static EditorObjectData CreateObjectData(EditorSplinePlacement placement)
	{
		vector transform[4];
		for (int i = 0; i < 4; i++) {
			transform[i] = placement.Transform[i];
		}

		float scale = transform[0].Length();
		Math3D.MatrixOrthogonalize4(transform);

		EditorObjectData data = new EditorObjectData();
		data.Type = placement.Type;
		data.Model = EditorObjectData.GetModelName(placement.Type);

		EditorSplineAssetInfo asset_info = EditorSplineAssetCatalog.Get(placement.Type);
		if (asset_info && asset_info.Model != string.Empty) {
			data.Model = asset_info.Model;
		}

		data.Position = transform[3];
		data.Orientation = Math3D.MatrixToAngles(transform);
		data.Scale = scale;
		data.Flags = EFE_DEFAULT;
		data.DisplayName = data.Type;
		return data;
	}
}