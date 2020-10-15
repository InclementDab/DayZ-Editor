
static const float MARKER_ALPHA_ON_SHOW = 1;
static const float MARKER_ALPHA_ON_HIDE = 0.25;

static const int DRAG_BOX_THRESHOLD = 15;
static const int DRAG_BOX_THICKNESS = 2;
static const int DRAG_BOX_COLOR = ARGB(100, 255, 0, 0);

static const int LIST_ITEM_COLOR_ON_SELECTED = COLOR_BLUE;
static const int LIST_ITEM_COLOR_ON_HOVER = COLOR_SALMON;
static const string LIST_ITEM_DEFAULT_ICON = "DayZEditor/gui/images/dayz_editor_icon_black.edds";

static DayZPlayer CreateDefaultCharacter(vector position = "0 0 0")
{
	EditorLog.Trace("CreateDefaultCharacter");
	DayZPlayer player;
	if (GetWorkbenchGame().GetPlayer() != null) {
		player = GetWorkbenchGame().GetPlayer();
	} 
	
	else if (Class.CastTo(player, GetWorkbenchGame().CreatePlayer(NULL, GetWorkbenchGame().CreateRandomPlayer(), position, 0, "NONE"))) {
		player.GetInventory().CreateInInventory("AviatorGlasses");
    	player.GetInventory().CreateInInventory("MilitaryBoots_Black");
    	player.GetInventory().CreateInInventory("AliceBag_Black");
	}

    return player;
}


static void SpawnStaticObject(string type, vector position, vector orientation)
{
	EditorLog.Trace("SpawnStaticObject %1", type);
    auto obj = GetGame().CreateObjectEx(type, position, ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS);
    obj.SetPosition(position);
    obj.SetOrientation(orientation);
    obj.SetOrientation(obj.GetOrientation());
    obj.Update();
}

/* Used for Offline Editor Mission Creation */
static string CreateEditorMission(string map_name = "ChernarusPlus")
{
	EditorLog.Trace("CreateEditorMission");
	string mission = "$saves:/Editor/Missions/DayZEditor." + map_name;
	
	if (!FileExist(mission)) {
		EditorLog.Info("Editor Mission not found, creating....");
		MakeDirectory(mission);
	}
	
	string init_file = mission + "/init.c";
	if (!FileExist(init_file)) {
		EditorLog.Info("Editor Init not found, creating....");
		CopyFile("DayZEditor/scripts/data/Defaults/Mission/init.c", init_file);
	}
	
	string map_group_file = mission + "/MapGroupProto.xml";
	if (!FileExist(map_group_file)) {
		EditorLog.Info("Editor MapGroupProto not found, creating....");
		CopyFile("DayZEditor/scripts/data/Defaults/Mission/MapGroupProto.xml", map_group_file);
	}
	
	return mission;
}