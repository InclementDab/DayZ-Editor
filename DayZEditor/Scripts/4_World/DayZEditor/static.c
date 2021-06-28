static const float MARKER_ALPHA_ON_SHOW = 1.0;
static const float MARKER_ALPHA_ON_HIDE = 1.0;

static const int DRAG_BOX_THRESHOLD = 15;
static const int DRAG_BOX_THICKNESS = 1;
static const int DRAG_BOX_COLOR = ARGB(255, 0, 120, 215);
static const int DRAG_BOX_FILL = ARGB(50, 0, 120, 215);

static const int LIST_ITEM_COLOR_ON_DELETED = COLOR_RED;
static const string LIST_ITEM_DEFAULT_ICON = "DayZEditor/gui/images/dayz_editor_icon_black.edds";

static const float BOUNDING_BOX_THICKNESS = 0.015;

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