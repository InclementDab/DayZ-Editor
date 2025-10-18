static const float MARKER_ALPHA_ON_SHOW = 1.0;
static const float MARKER_ALPHA_ON_HIDE = 0.8;
static const float MARKER_ALPHA_ON_INVUNERABLE = 0.3;

static const int DRAG_BOX_THRESHOLD = 15;
static const int DRAG_BOX_THICKNESS = 1;

static const int LIST_ITEM_COLOR_ON_DELETED = COLOR_RED;
static const string LIST_ITEM_DEFAULT_ICON = "DayZEditor/gui/images/dayz_editor_icon_black.edds";

/* Used for Offline Editor Mission Creation */
static string CreateEditorMissionFolder(string map_name = "ChernarusPlus")
{	
	string mission = "$saves:Editor/Missions/Editor." + map_name;
	
	if (!MakeDirectory("$saves:Editor/")) {
		EditorLog.Error("Failed to create Editor Directory");
		return mission;
	}
	
	if (!MakeDirectory("$saves:Editor/Missions/")) {
		EditorLog.Error("Failed to create Editor Mission Directory");
		return mission;
	}
	
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