// Resolves a config class, model path or model file name once: spawn type, console friendly, size and road points
class EditorSplineAssetCatalog
{
	protected static ref map<string, ref EditorSplineAssetInfo> s_AssetInfos;
	protected static ref map<string, EditorPlaceableItem> s_StaticByPath;
	protected static ref map<string, EditorPlaceableItem> s_StaticByFile;

	// lower case, back slashes, no leading slash
	static string Normalize(string name)
	{
		string normalized = name.Trim();
		normalized.ToLower();
		normalized.Replace("/", "\\");
		while (normalized.Length() > 0 && normalized.Substring(0, 1) == "\\") {
			normalized = normalized.Substring(1, normalized.Length() - 1);
		}

		return normalized;
	}

	static void Reset()
	{
		s_AssetInfos = null;
		s_StaticByPath = null;
		s_StaticByFile = null;
	}

	protected static void BuildStaticIndex()
	{
		s_StaticByPath = new map<string, EditorPlaceableItem>();
		s_StaticByFile = new map<string, EditorPlaceableItem>();

		array<ref EditorPlaceableItem> placeable_items = GetEditor().GetPlaceableObjects();
		foreach (EditorPlaceableItem placeable_item: placeable_items) {
			if (!placeable_item || placeable_item.Category != EditorPlaceableItemCategory.STATIC) {
				continue;
			}

			string path_key = Normalize(placeable_item.Path);
			if (!s_StaticByPath.Contains(path_key)) {
				s_StaticByPath.Insert(path_key, placeable_item);
			}

			string file_key = GetFileKey(path_key);
			if (!s_StaticByFile.Contains(file_key)) {
				s_StaticByFile.Insert(file_key, placeable_item);
			}
		}
	}

	protected static string GetFileKey(string path)
	{
		int slash = path.LastIndexOf("\\");
		if (slash == -1) {
			return path;
		}

		return path.Substring(slash + 1, path.Length() - slash - 1);
	}

	// The editors own placeable item of a config class, model path or model file name
	static EditorPlaceableItem FindPlaceableItem(string name)
	{
		if (!GetEditor() || name == string.Empty) {
			return null;
		}

		EditorPlaceableItem config_item = GetEditor().GetPlaceableObject(name);
		if (config_item) {
			return config_item;
		}

		if (!s_StaticByPath) {
			BuildStaticIndex();
		}

		string key = Normalize(name);
		EditorPlaceableItem static_item;
		if (s_StaticByPath.Find(key, static_item)) {
			return static_item;
		}

		string file_key = GetFileKey(key);
		if (file_key.IndexOf(".p3d") == -1) {
			file_key += ".p3d";
		}

		if (s_StaticByFile.Find(file_key, static_item)) {
			return static_item;
		}

		return null;
	}

	static EditorSplineAssetInfo Get(string name)
	{
		string key = name.Trim();
		if (key == string.Empty) {
			return null;
		}

		if (!s_AssetInfos) {
			s_AssetInfos = new map<string, ref EditorSplineAssetInfo>();
		}

		EditorSplineAssetInfo cached_info;
		if (s_AssetInfos.Find(key, cached_info)) {
			return cached_info;
		}

		EditorSplineAssetInfo asset_info = new EditorSplineAssetInfo();
		asset_info.Name = key;
		s_AssetInfos.Insert(key, asset_info);

		EditorPlaceableItem placeable_item = FindPlaceableItem(key);
		if (placeable_item) {
			asset_info.SpawnType = placeable_item.GetSpawnType();
			asset_info.ConsoleFriendly = placeable_item.ConsoleFriendly;
			asset_info.IsStatic = placeable_item.Category == EditorPlaceableItemCategory.STATIC;
		} else if (GetGame().ConfigIsExisting(CFG_VEHICLESPATH + " " + key)) {
			asset_info.SpawnType = key;
			string model = Normalize(GetGame().ConfigGetTextOut(CFG_VEHICLESPATH + " " + key + " model"));
			asset_info.ConsoleFriendly = model.IndexOf("dz\\") == 0;
		} else if (key.Contains(".p3d")) {
			asset_info.SpawnType = key;
			asset_info.IsStatic = true;
			asset_info.ConsoleFriendly = EditorObjectManagerModule.ValidatePath(key);
		} else {
			return asset_info;
		}

		Measure(asset_info);
		return asset_info;
	}

	static bool IsUsable(EditorSplineAssetInfo asset_info, bool console_only)
	{
		if (!asset_info || !asset_info.Valid) {
			return false;
		}

		return !console_only || asset_info.ConsoleFriendly;
	}

	// Spawns a local copy at the origin, reads its size and memory points and deletes it again
	protected static void Measure(EditorSplineAssetInfo asset_info)
	{
		Object probe;
		if (asset_info.IsStatic) {
			string p3d_path = SystemPath.Format(asset_info.SpawnType);
			p3d_path.Replace("\\", "/");
			while (p3d_path.Length() > 0 && p3d_path.Substring(0, 1) == "/") {
				p3d_path = p3d_path.Substring(1, p3d_path.Length() - 1);
			}

			probe = GetGame().CreateStaticObjectUsingP3D(p3d_path, vector.Zero, vector.Zero, 1.0, true);
		} else {
			probe = GetGame().CreateObjectEx(asset_info.SpawnType, vector.Zero, ECE_LOCAL | ECE_SETUP | ECE_CREATEPHYSICS | ECE_NOLIFETIME | ECE_NOPERSISTENCY_CHAR | ECE_NOPERSISTENCY_WORLD);
		}

		if (!probe) {
			asset_info.Valid = false;
			return;
		}

		vector clip[2];
		if (probe.IsItemBase()) {
			vector item_min, item_max;
			probe.GetActionComponentMinMax(probe.GetViewGeometryLevel(), 0, item_min, item_max);
			clip[0] = item_min;
			clip[1] = item_max;
		} else {
			probe.ClippingInfo(clip);
		}

		asset_info.ClipMin = clip[0];
		asset_info.ClipMax = clip[1];

		// clipping box is padded (a 4m wall reports 6m), the collision box is the real size. decals and roads have none
		vector collision[2];
		if (probe.GetCollisionBox(collision)) {
			float collision_side = Math.Max(collision[1][0] - collision[0][0], collision[1][2] - collision[0][2]);
			if (collision_side > 0.3) {
				asset_info.ClipMin = collision[0];
				asset_info.ClipMax = collision[1];
			}
		}

		if (asset_info.GetLongestSide() < 0.05) {
			asset_info.ClipMin = Vector(-0.5, 0, -0.5);
			asset_info.ClipMax = Vector(0.5, 1, 0.5);
		}

		asset_info.Model = probe.GetShapeName();

		// begin and end edge of road parts, the same points terrain builder joins them with
		if (probe.MemoryPointExists("lb") && probe.MemoryPointExists("pb") && probe.MemoryPointExists("le") && probe.MemoryPointExists("pe")) {
			vector left_begin = probe.GetMemoryPointPos("lb");
			vector right_begin = probe.GetMemoryPointPos("pb");
			vector left_end = probe.GetMemoryPointPos("le");
			vector right_end = probe.GetMemoryPointPos("pe");

			asset_info.RoadBegin = (left_begin + right_begin) * 0.5;
			asset_info.RoadEnd = (left_end + right_end) * 0.5;

			vector chord = asset_info.RoadEnd - asset_info.RoadBegin;
			chord[1] = 0;
			vector begin_edge = right_begin - left_begin;
			begin_edge[1] = 0;
			vector end_edge = right_end - left_end;
			end_edge[1] = 0;

			asset_info.RoadBeginDirection = GetEdgeNormal(begin_edge, chord);
			asset_info.RoadEndDirection = GetEdgeNormal(end_edge, chord);
			asset_info.RoadWidth = begin_edge.Length();
			asset_info.HasRoadPoints = chord.Length() > 0.5;
		}

		GetGame().ObjectDelete(probe);
		asset_info.Valid = true;
		EditorLog.Debug("EditorSplineAssetCatalog::Measure %1 %2 .. %3, road points %4, console friendly %5", asset_info.SpawnType, asset_info.ClipMin.ToString(), asset_info.ClipMax.ToString(), asset_info.HasRoadPoints.ToString(), asset_info.ConsoleFriendly.ToString());
	}

	// Direction across an edge, on the side the road continues to
	protected static vector GetEdgeNormal(vector edge, vector chord)
	{
		if (edge.LengthSq() < 0.0001) {
			return EditorSplineMath.Flatten(chord);
		}

		vector normal = Vector(edge[2], 0, -edge[0]);
		normal.Normalize();
		if (vector.Dot(normal, chord) < 0) {
			normal = -normal;
		}

		return normal;
	}
}