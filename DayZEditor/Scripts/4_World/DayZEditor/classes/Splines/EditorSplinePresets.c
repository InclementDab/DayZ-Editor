class EditorSplinePresets
{
	static const int VERSION = 1;
	
	// Sub folder the next presets go to, empty for the type folder itself
	protected static string s_Folder;
	
	static string GetDirectory()
	{
		return SystemPath.Combine(Editor.ROOT_DIRECTORY, "Generators");
	}
	
	static string GetDirectory(EditorSplineGeneratorType type)
	{
		string folder_name = EditorSplineGeneratorSettings.GetFolderName(type);
		if (folder_name == string.Empty) {
			return GetDirectory();
		}
		
		return SystemPath.Combine(GetDirectory(), folder_name);
	}
	
	static EditorSplineGeneratorSettings Load(string file)
	{
		EditorSplineGeneratorSettings settings;
		string error;
		if (!JsonFileLoader<EditorSplineGeneratorSettings>.LoadFile(file, settings, error)) {
			EditorLog.Warning(error);
			return null;
		}
		
		settings.Repair();
		return settings;
	}
	
	static bool Save(string file, EditorSplineGeneratorSettings settings)
	{
		string error;
		if (!JsonFileLoader<EditorSplineGeneratorSettings>.SaveFile(file, settings, error)) {
			EditorLog.Warning(error);
			return false;
		}
		
		return true;
	}
	
	// Written once, presets that get deleted or edited afterwards are left alone
	static void CreateDefaults()
	{
		EditorSettings settings = GetEditor().GetSettings();
		if (settings.GeneratorPresetVersion >= VERSION) {
			return;
		}
		
		AddRoad("Asphalt Highway (asf1)", "asf1");
		AddRoad("Asphalt Road (asf2)", "asf2");
		AddRoad("Asphalt Narrow Road (asf3)", "asf3");
		AddRoad("City Street", "city");
		AddRoad("Gravel Road", "grav");
		AddRoad("Dirt Road", "mud");
		AddRoad("Airfield Taxiway", "taxiway");
		AddRoad("Forest Path - Dirt", "path_dirt");
		AddRoad("Forest Path - Rock", "path_rock");

		AddWall("Concrete Wall 4m (IndCnc)", "StaticObj_Wall_IndCnc_4", "StaticObj_Wall_IndCnc_Pole", EditorSplinePoleMode.CORNERS);
		AddWall("Concrete Wall 10m (IndCnc)", "StaticObj_Wall_IndCnc_10", "StaticObj_Wall_IndCnc_Pole", EditorSplinePoleMode.CORNERS);
		AddWall("Military Fence 9m (IndFnc)", "StaticObj_Wall_IndFnc_9", "StaticObj_Wall_IndFnc_Pole", EditorSplinePoleMode.CORNERS);
		AddWall("Chain Link Fence 3m (IndFnc)", "StaticObj_Wall_IndFnc_3", "StaticObj_Wall_IndFnc_Pole", EditorSplinePoleMode.EVERY_PIECE);
		AddWall("Industrial Fence 5m (IndVar1)", "StaticObj_Wall_IndVar1_5", "StaticObj_Wall_IndVar1_Pole", EditorSplinePoleMode.EVERY_PIECE);
		AddWall("Brick Wall 5m (CBrk)", "StaticObj_Wall_CBrk_5", "StaticObj_Wall_CBrk_Pole", EditorSplinePoleMode.EVERY_PIECE);
		AddWall("Grey Concrete Wall 5m (CGry)", "StaticObj_Wall_CGry_5", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Small Concrete Wall 8m", "StaticObj_Wall_CncSmall_8", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Village Fence 4m (VilVar2)", "StaticObj_Wall_VilVar2_4", "StaticObj_Wall_VilVar2_Pole", EditorSplinePoleMode.EVERY_PIECE);
		AddWall("Wooden Fence 5m (Wood1)", "StaticObj_Wall_Wood1_5", "StaticObj_Wall_Wood1_5_pole", EditorSplinePoleMode.EVERY_PIECE);
		AddWall("Wooden Fence 4m (Wood4)", "StaticObj_Wall_Wood4_4", "StaticObj_Wall_Wood4_Pole", EditorSplinePoleMode.EVERY_PIECE);
		AddWall("Tin Fence 5m", "StaticObj_Wall_Tin_5", "StaticObj_Wall_Tin_Pole", EditorSplinePoleMode.EVERY_PIECE);
		AddWall("Stone Wall", "StaticObj_Wall_Stone", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Crash Barrier (guard rail)", "StaticObj_Misc_CrashBarrier", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Pipe Fence 8m (Livonia)", "StaticObj_Wall_PipeFence_8", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Military Concrete Wall 4m (Livonia)", "StaticObj_Wall_MilCnc_4", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Concrete Barriers (Livonia)", "StaticObj_Wall_CncBarrier_4Block", string.Empty, EditorSplinePoleMode.NONE);

		AddForest("Spruce Forest", EditorSplineAssetLists.SPRUCE_TREES, EditorSplineAssetLists.BUSHES, EditorSplineAssetLists.ROCKS, 300, 70);
		AddForest("Pine Forest", EditorSplineAssetLists.PINE_TREES, EditorSplineAssetLists.BUSHES, EditorSplineAssetLists.ROCKS, 260, 60);
		AddForest("Mixed Forest", EditorSplineAssetLists.MIXED_TREES, EditorSplineAssetLists.BUSHES, EditorSplineAssetLists.ROCKS, 250, 80);
		AddForest("Birch Grove", EditorSplineAssetLists.BIRCH_TREES, EditorSplineAssetLists.BUSHES, EditorSplineAssetLists.ROCKS, 220, 90);
		AddForest("Beech and Oak Forest", EditorSplineAssetLists.DECIDUOUS_TREES, EditorSplineAssetLists.BUSHES, EditorSplineAssetLists.ROCKS, 200, 90);
		AddForest("Livonia Forest", EditorSplineAssetLists.LIVONIA_TREES, EditorSplineAssetLists.LIVONIA_BUSHES, EditorSplineAssetLists.ROCKS, 250, 80);
		AddForest("Bushes Only (hedge)", null, EditorSplineAssetLists.BUSHES, null, 0, 500);

		AddAsset("Wooden Power Poles", "Land_Power_Pole_Wood1_Amp", 40, 0, false);
		AddAsset("Concrete Power Poles", "Land_Power_Pole_Conc1_Amp", 45, 0, false);
		AddAsset("High Voltage Towers", "StaticObj_Power_HV_Tower", 120, 0, false);
		AddAsset("Street Lamps (both sides)", "StaticObj_Lamp_City1", 30, 6, true);
		AddAsset("Road Barriers", "StaticObj_Misc_RoadBarrier", 4, 0, false);

		EditorSplineGeneratorSettings avenue = new EditorSplineGeneratorSettings();
		avenue.Type = EditorSplineGeneratorType.ASSET;
		avenue.Assets.Copy(EditorSplineAssetLists.BIRCH_TREES);
		avenue.PickMode = EditorSplinePickMode.RANDOM;
		avenue.Spacing = 12;
		avenue.SideOffset = 7;
		avenue.BothSides = true;
		avenue.DirectionMode = EditorSplineDirectionMode.RANDOM;
		avenue.ScaleMin = 0.9;
		avenue.ScaleMax = 1.15;
		avenue.PositionJitter = 0.6;
		SavePreset(avenue, "Tree Avenue (both sides)");

		AddFootpath("Dirt Footpath", EditorSplineAssetLists.FOOTPATH_DIRT, 0.3, 0, 0);
		AddFootpath("Rocky Footpath", EditorSplineAssetLists.FOOTPATH_ROCK, 0.3, 0, 0);
		AddFootpath("Worn Dirt Trail", EditorSplineAssetLists.FOOTPATH_TRAIL, 0.6, 4, 0.25);
		AddFootpath("Concrete Panel Path", EditorSplineAssetLists.FOOTPATH_PANELS, 0.05, 0, 0);
		AddFootpath("Sidewalk", EditorSplineAssetLists.FOOTPATH_SIDEWALK, 0.02, 0, 0);

		s_Folder = "Nasdara";
		AddFootpath("Nasdara Dirt Trail", EditorSplineAssetLists.NASDARA_TRAIL, 0.6, 4, 0.25);
		AddFootpath("Nasdara Sidewalk 6", EditorSplineAssetLists.NASDARA_SIDEWALK_6, 0.02, 0, 0);
		AddFootpath("Nasdara Sidewalk 7", EditorSplineAssetLists.NASDARA_SIDEWALK_7, 0.02, 0, 0);
		AddFootpath("Nasdara Sidewalk 8", EditorSplineAssetLists.NASDARA_SIDEWALK_8, 0.02, 0, 0);

		AddWall("Nasdara Concrete Wall 4m", "StaticObj_Wall_IndCnc_Desert_4", "StaticObj_Wall_IndCnc_Desert_Pole", EditorSplinePoleMode.CORNERS);
		AddWall("Nasdara Concrete Wall 10m", "StaticObj_Wall_IndCnc_Desert_10", "StaticObj_Wall_IndCnc_Desert_Pole", EditorSplinePoleMode.CORNERS);
		AddWall("Nasdara Industrial Wall 8m", "StaticObj_Wall_IndCnc4_Desert_8", "StaticObj_Wall_IndCnc4_Desert_Pole", EditorSplinePoleMode.EVERY_PIECE);
		AddWall("Nasdara Industrial Wall Low 8m", "StaticObj_Wall_IndCnc4_Desert_Low_8", "StaticObj_Wall_IndCnc4_Desert_Low_Pole", EditorSplinePoleMode.EVERY_PIECE);
		AddWall("Nasdara High Concrete Wall 10m", "StaticObj_Wall_IndCncH_Desert_10", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Nasdara Small Concrete Wall 8m", "StaticObj_Wall_CncSmall_Desert_8", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Nasdara Chain Link Fence 9m", "StaticObj_Wall_IndFnc2_Desert_9", "StaticObj_Wall_IndFnc2_Desert_Pole", EditorSplinePoleMode.CORNERS);
		AddWall("Nasdara Mud Brick Wall 5m (M1)", "StaticObj_Wall_M1_5", "StaticObj_Wall_M1_Pole", EditorSplinePoleMode.EVERY_PIECE);
		AddWall("Nasdara Mud Wall 5m (M3)", "StaticObj_Wall_M3_5", "StaticObj_Wall_M3_Pole", EditorSplinePoleMode.EVERY_PIECE);
		AddWall("Nasdara Mud Wall 10m (M4)", "StaticObj_Wall_M4_10_1", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Nasdara Stone Wall 10m (M5)", "StaticObj_Wall_M5_10", "StaticObj_Wall_M5_Pole", EditorSplinePoleMode.EVERY_PIECE);
		AddWall("Nasdara Block Wall 5m (B1)", "StaticObj_Wall_B1_5", "StaticObj_Wall_B1_Pole", EditorSplinePoleMode.EVERY_PIECE);
		AddWall("Nasdara Block Wall 5m (B2)", "StaticObj_Wall_B2_5", "StaticObj_Wall_B2_Pole", EditorSplinePoleMode.EVERY_PIECE);
		AddWall("Nasdara Plaster Wall 8m (S1)", "StaticObj_Wall_S1_8", "StaticObj_Wall_S1_Pole", EditorSplinePoleMode.EVERY_PIECE);
		AddWall("Nasdara T-Walls High", "StaticObj_Wall_Bremer_High_8", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Nasdara T-Walls Medium", "StaticObj_Wall_Bremer_Medium_8", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Nasdara T-Walls Low", "StaticObj_Wall_Bremer_Low_8", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Nasdara Wired Fence", "StaticObj_Mil_WiredFence", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Nasdara Pipe Fence 8m", "StaticObj_PipeFence_01_m_8m", "StaticObj_PipeFence_01_m_pole", EditorSplinePoleMode.EVERY_PIECE);
		AddWall("Nasdara Stone Wall", "StaticObj_Wall_Stone3", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Nasdara Wooden Fence 6m", "StaticObj_Wall_Wood6_6", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Nasdara Crash Barrier 8m", "StaticObj_Misc_CrashBarrier_01_8m", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Nasdara Double Crash Barrier 8m", "StaticObj_Misc_CrashBarrier_01_Double_8m", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Nasdara Jersey Barriers", "StaticObj_Misc_JerseyBarrier", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Nasdara Eroded Embankment (dirt)", "DZ/rocks_takistan/ErodedEmbankment_01_dirt_straight.p3d", string.Empty, EditorSplinePoleMode.NONE);
		AddWall("Nasdara Eroded Embankment (grass)", "DZ/rocks_takistan/ErodedEmbankment_01_grass_straight.p3d", string.Empty, EditorSplinePoleMode.NONE);

		AddForest("Nasdara Palm Grove", EditorSplineAssetLists.NASDARA_PALMS, EditorSplineAssetLists.NASDARA_BUSHES, EditorSplineAssetLists.NASDARA_STONES, 120, 80);
		AddForest("Nasdara Olive Orchard", EditorSplineAssetLists.NASDARA_OLIVES, null, EditorSplineAssetLists.NASDARA_STONES, 90, 0);
		AddForest("Nasdara Pine Woods", EditorSplineAssetLists.NASDARA_PINES, EditorSplineAssetLists.NASDARA_BUSHES, EditorSplineAssetLists.NASDARA_STONES, 200, 70);
		AddForest("Nasdara Oasis", EditorSplineAssetLists.NASDARA_OASIS_TREES, EditorSplineAssetLists.NASDARA_BUSHES, EditorSplineAssetLists.NASDARA_STONES, 180, 120);
		AddForest("Nasdara Desert Scrub", null, EditorSplineAssetLists.NASDARA_SCRUB, EditorSplineAssetLists.NASDARA_STONES, 0, 150, 15);
		AddForest("Nasdara Rock Field", null, null, EditorSplineAssetLists.NASDARA_BOULDERS, 0, 0, 40);

		AddAsset("Nasdara Bollards", "StaticObj_Bollard_01", 1.5, 0, false);

		EditorSplineGeneratorSettings palm_avenue = new EditorSplineGeneratorSettings();
		palm_avenue.Type = EditorSplineGeneratorType.ASSET;
		palm_avenue.Assets.Copy(EditorSplineAssetLists.NASDARA_PALMS);
		palm_avenue.PickMode = EditorSplinePickMode.RANDOM;
		palm_avenue.Spacing = 10;
		palm_avenue.SideOffset = 6;
		palm_avenue.BothSides = true;
		palm_avenue.DirectionMode = EditorSplineDirectionMode.RANDOM;
		palm_avenue.ScaleMin = 0.9;
		palm_avenue.ScaleMax = 1.15;
		SavePreset(palm_avenue, "Nasdara Palm Avenue (both sides)");

		// bare road models, they place in the editor but no object spawner will spawn them
		s_Folder = "Nasdara (not console safe)";
		string parts = "DZ/structures_takistan/Roads/Parts/";
		array<string> full_set = { "25", "12", "6", "02000", "11000", "7100", "1575", "2250", "3025", "6010" };
		array<string> straight_set = { "25", "12", "6" };
		array<string> sand_set = { "30", "10", "curve" };
		AddRoadParts("Nasdara Highway", parts + "asf1takistan_", full_set);
		AddRoadParts("Nasdara Road", parts + "asf2takistan_", full_set);
		AddRoadParts("Nasdara Narrow Road", parts + "asf3takistan_", straight_set);
		AddRoadParts("Nasdara Industrial Road", parts + "asf1IndustrialFaded_", straight_set);
		AddRoadParts("Nasdara Dirt Road", parts + "dirt1takistan_", full_set);
		AddRoadParts("Nasdara Dirt Road 2", parts + "dirt2takistan_", full_set);
		AddRoadParts("Nasdara Dirt Track", parts + "dirt3takistan_", straight_set);
		AddRoadParts("Nasdara Sand Path", parts + "path_sand_", sand_set);

		s_Folder = string.Empty;
		
		settings.GeneratorPresetVersion = VERSION;
		settings.Save();
	}

	protected static void AddRoad(string name, string kit)
	{
		EditorSplineGeneratorSettings settings = new EditorSplineGeneratorSettings();
		settings.Type = EditorSplineGeneratorType.ROAD;
		settings.RoadKit = kit;
		SavePreset(settings, name);
	}
	
	protected static void AddRoadParts(string name, string prefix, array<string> suffixes)
	{
		EditorSplineGeneratorSettings settings = new EditorSplineGeneratorSettings();
		settings.Type = EditorSplineGeneratorType.ROAD;
		settings.RoadKit = "custom";
		foreach (string suffix: suffixes) {
			settings.RoadPieces.Insert(prefix + suffix + ".p3d");
		}

		SavePreset(settings, name);
	}

	protected static void AddWall(string name, string wall, string pole, EditorSplinePoleMode pole_mode)
	{
		EditorSplineGeneratorSettings settings = new EditorSplineGeneratorSettings();
		settings.Type = EditorSplineGeneratorType.WALL;
		settings.Wall = wall;
		settings.Pole = pole;
		settings.PoleMode = pole_mode;
		SavePreset(settings, name);
	}

	protected static void AddFootpath(string name, array<string> pieces, float overlap, float yaw_jitter, float side_jitter)
	{
		EditorSplineGeneratorSettings settings = new EditorSplineGeneratorSettings();
		settings.Type = EditorSplineGeneratorType.FOOTPATH;
		settings.PathPieces.Copy(pieces);
		settings.PickMode = EditorSplinePickMode.RANDOM;
		settings.Overlap = overlap;
		settings.YawJitter = yaw_jitter;
		settings.PositionJitter = side_jitter;
		settings.AlignToTerrain = true;
		settings.HeightOffset = 0.02;
		SavePreset(settings, name);
	}

	protected static void AddForest(string name, array<string> trees, array<string> bushes, array<string> rocks, float tree_density, float bush_density, float rock_density = 6)
	{
		EditorSplineGeneratorSettings settings = new EditorSplineGeneratorSettings();
		settings.Type = EditorSplineGeneratorType.FOREST;
		if (trees) {
			settings.Trees.Copy(trees);
		}

		if (bushes) {
			settings.Bushes.Copy(bushes);
		}

		if (rocks) {
			settings.Rocks.Copy(rocks);
		}

		settings.TreeDensity = tree_density;
		settings.BushDensity = bush_density;
		settings.RockDensity = rock_density;
		settings.ScaleMin = 0.85;
		settings.ScaleMax = 1.2;
		SavePreset(settings, name);
	}

	protected static void AddAsset(string name, string asset, float spacing, float side_offset, bool both_sides)
	{
		EditorSplineGeneratorSettings settings = new EditorSplineGeneratorSettings();
		settings.Type = EditorSplineGeneratorType.ASSET;
		settings.Assets.Insert(asset);
		settings.Spacing = spacing;
		settings.SideOffset = side_offset;
		settings.BothSides = both_sides;
		if (both_sides) {
			settings.DirectionMode = EditorSplineDirectionMode.SIDEWAYS;
		}

		SavePreset(settings, name);
	}

	protected static void SavePreset(EditorSplineGeneratorSettings settings, string name)
	{
		MakeDirectory(GetDirectory());
		string directory = GetDirectory(settings.Type);
		MakeDirectory(directory);
		if (s_Folder != string.Empty) {
			directory = SystemPath.Combine(directory, s_Folder);
			MakeDirectory(directory);
		}

		string file = SystemPath.Combine(directory, name + EditorSplinePresetFile.EXTENSION);
		if (FileExist(file)) {
			return;
		}

		settings.PresetName = name;
		Save(file, settings);
	}
}