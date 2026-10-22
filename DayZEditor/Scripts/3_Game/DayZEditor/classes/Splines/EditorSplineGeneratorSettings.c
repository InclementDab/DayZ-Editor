// One settings object per spline, only the fields of the chosen generator are used
class EditorSplineGeneratorSettings: Managed
{
	// prefabs bound to these settings call PropertyChanged
	static ref ScriptInvoker OnPropertyChanged = new ScriptInvoker();

	EditorSplineGeneratorType Type;
	string PresetName;

	// Shared
	float HeightOffset;
	bool SnapToTerrain = true;
	float ScaleMin = 1.0;
	float ScaleMax = 1.0;
	int Seed = 1;

	// Asset
	ref array<string> Assets = {};
	EditorSplinePickMode PickMode;
	EditorSplinePlaceMode PlaceMode;
	float Spacing = 10.0;
	float StartOffset;
	EditorSplineDirectionMode DirectionMode;
	float YawOffset;
	float YawJitter;
	float SideOffset;
	bool BothSides;
	float PositionJitter;
	bool AlignToTerrain;

	// Road, RoadKit is the vanilla road family or "custom" to use RoadPieces
	string RoadKit = "asf2";
	ref array<string> RoadPieces = {};
	bool RoadCurves = true;
	float MaxDeviation = 0.75;

	// Wall
	string Wall;
	string Pole;
	EditorSplinePoleMode PoleMode;
	EditorSplineFitMode FitMode;
	float LengthOverride;
	float Overlap;
	bool WallFlip;
	bool FollowSlope;
	float Sink = 0.1;

	// Footpath
	ref array<string> PathPieces = {};

	// Forest
	ref array<string> Trees = {};
	ref array<string> Bushes = {};
	ref array<string> Rocks = {};
	float TreeDensity = 250.0;
	float BushDensity = 80.0;
	float RockDensity = 6.0;
	float MinDistance = 3.5;
	float EdgeWidth = 10.0;
	float EdgeBushBoost = 2.5;
	float MaxSlope = 40.0;
	bool AvoidWater = true;
	float TreeSink = 0.15;
	float RockSink = 0.35;
	bool AlignRocks = true;

	void PropertyChanged(string property_name)
	{
		OnPropertyChanged.Invoke(this, property_name);
	}

	void Repair()
	{
		if (!Assets) {
			Assets = {};
		}

		if (!RoadPieces) {
			RoadPieces = {};
		}

		if (!PathPieces) {
			PathPieces = {};
		}

		if (!Trees) {
			Trees = {};
		}

		if (!Bushes) {
			Bushes = {};
		}

		if (!Rocks) {
			Rocks = {};
		}

		if (RoadKit == string.Empty) {
			RoadKit = "asf2";
		}

		if (ScaleMax < ScaleMin) {
			float scale_min = ScaleMax;
			ScaleMax = ScaleMin;
			ScaleMin = scale_min;
		}
	}

	// Something sensible to start with, existing values are kept
	void ApplyTypeDefaults(EditorSplineGeneratorType type)
	{
		Type = type;
		switch (type) {
			case EditorSplineGeneratorType.WALL: {
				if (Wall == string.Empty) {
					Wall = "StaticObj_Wall_IndCnc_4";
					Pole = "StaticObj_Wall_IndCnc_Pole";
					PoleMode = EditorSplinePoleMode.CORNERS;
				}

				break;
			}

			case EditorSplineGeneratorType.FOOTPATH: {
				if (PathPieces.Count() == 0) {
					PathPieces.Copy(EditorSplineAssetLists.FOOTPATH_DIRT);
					Overlap = 0.3;
					AlignToTerrain = true;
					HeightOffset = 0.02;
				}

				break;
			}

			case EditorSplineGeneratorType.FOREST: {
				if (Trees.Count() == 0 && Bushes.Count() == 0 && Rocks.Count() == 0) {
					Trees.Copy(EditorSplineAssetLists.MIXED_TREES);
					Bushes.Copy(EditorSplineAssetLists.BUSHES);
					Rocks.Copy(EditorSplineAssetLists.ROCKS);
					ScaleMin = 0.85;
					ScaleMax = 1.2;
				}

				break;
			}
		}
	}

	static string GetTypeName(EditorSplineGeneratorType type)
	{
		switch (type) {
			case EditorSplineGeneratorType.ASSET: return "Asset";
			case EditorSplineGeneratorType.ROAD: return "Road";
			case EditorSplineGeneratorType.WALL: return "Wall";
			case EditorSplineGeneratorType.FOREST: return "Forest";
			case EditorSplineGeneratorType.FOOTPATH: return "Footpath";
		}

		return "None";
	}

	// Preset folder of each type
	static string GetFolderName(EditorSplineGeneratorType type)
	{
		switch (type) {
			case EditorSplineGeneratorType.ASSET: return "Assets";
			case EditorSplineGeneratorType.ROAD: return "Roads";
			case EditorSplineGeneratorType.WALL: return "Walls";
			case EditorSplineGeneratorType.FOREST: return "Forests";
			case EditorSplineGeneratorType.FOOTPATH: return "Footpaths";
		}

		return string.Empty;
	}

	// asf2 -> StaticObj_Roads_Partasf2_, taxiway -> StaticObj_Taxiway_
	static string GetRoadKitPrefix(string kit)
	{
		string kit_lower = kit;
		kit_lower.ToLower();
		if (kit_lower == "taxiway") {
			return "StaticObj_Taxiway_";
		}

		return "StaticObj_Roads_Part" + kit + "_";
	}

	void Write(Serializer serializer, int version)
	{
		serializer.Write(Type);
		serializer.Write(PresetName);

		serializer.Write(HeightOffset);
		serializer.Write(SnapToTerrain);
		serializer.Write(ScaleMin);
		serializer.Write(ScaleMax);
		serializer.Write(Seed);

		WriteList(serializer, Assets);
		serializer.Write(PickMode);
		serializer.Write(PlaceMode);
		serializer.Write(Spacing);
		serializer.Write(StartOffset);
		serializer.Write(DirectionMode);
		serializer.Write(YawOffset);
		serializer.Write(YawJitter);
		serializer.Write(SideOffset);
		serializer.Write(BothSides);
		serializer.Write(PositionJitter);
		serializer.Write(AlignToTerrain);

		serializer.Write(RoadKit);
		WriteList(serializer, RoadPieces);
		serializer.Write(RoadCurves);
		serializer.Write(MaxDeviation);

		serializer.Write(Wall);
		serializer.Write(Pole);
		serializer.Write(PoleMode);
		serializer.Write(FitMode);
		serializer.Write(LengthOverride);
		serializer.Write(Overlap);
		serializer.Write(WallFlip);
		serializer.Write(FollowSlope);
		serializer.Write(Sink);

		WriteList(serializer, PathPieces);

		WriteList(serializer, Trees);
		WriteList(serializer, Bushes);
		WriteList(serializer, Rocks);
		serializer.Write(TreeDensity);
		serializer.Write(BushDensity);
		serializer.Write(RockDensity);
		serializer.Write(MinDistance);
		serializer.Write(EdgeWidth);
		serializer.Write(EdgeBushBoost);
		serializer.Write(MaxSlope);
		serializer.Write(AvoidWater);
		serializer.Write(TreeSink);
		serializer.Write(RockSink);
		serializer.Write(AlignRocks);
	}

	bool Read(Serializer serializer, int version)
	{
		serializer.Read(Type);
		serializer.Read(PresetName);

		serializer.Read(HeightOffset);
		serializer.Read(SnapToTerrain);
		serializer.Read(ScaleMin);
		serializer.Read(ScaleMax);
		serializer.Read(Seed);

		ReadList(serializer, Assets);
		serializer.Read(PickMode);
		serializer.Read(PlaceMode);
		serializer.Read(Spacing);
		serializer.Read(StartOffset);
		serializer.Read(DirectionMode);
		serializer.Read(YawOffset);
		serializer.Read(YawJitter);
		serializer.Read(SideOffset);
		serializer.Read(BothSides);
		serializer.Read(PositionJitter);
		serializer.Read(AlignToTerrain);

		serializer.Read(RoadKit);
		ReadList(serializer, RoadPieces);
		serializer.Read(RoadCurves);
		serializer.Read(MaxDeviation);

		serializer.Read(Wall);
		serializer.Read(Pole);
		serializer.Read(PoleMode);
		serializer.Read(FitMode);
		serializer.Read(LengthOverride);
		serializer.Read(Overlap);
		serializer.Read(WallFlip);
		serializer.Read(FollowSlope);
		serializer.Read(Sink);

		ReadList(serializer, PathPieces);

		ReadList(serializer, Trees);
		ReadList(serializer, Bushes);
		ReadList(serializer, Rocks);
		serializer.Read(TreeDensity);
		serializer.Read(BushDensity);
		serializer.Read(RockDensity);
		serializer.Read(MinDistance);
		serializer.Read(EdgeWidth);
		serializer.Read(EdgeBushBoost);
		serializer.Read(MaxSlope);
		serializer.Read(AvoidWater);
		serializer.Read(TreeSink);
		serializer.Read(RockSink);

		if (!serializer.Read(AlignRocks)) {
			return false;
		}

		Repair();
		return true;
	}

	protected static void WriteList(Serializer serializer, array<string> list)
	{
		serializer.Write(list.Count());
		foreach (string entry: list) {
			serializer.Write(entry);
		}
	}

	protected static void ReadList(Serializer serializer, array<string> list)
	{
		list.Clear();

		int count;
		serializer.Read(count);
		for (int i = 0; i < count; i++) {
			string entry;
			serializer.Read(entry);
			list.Insert(entry);
		}
	}
}