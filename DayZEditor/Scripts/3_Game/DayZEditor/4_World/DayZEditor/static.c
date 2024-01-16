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
	
	if (!MakeDirectory("$saves:/Editor/")) {
		EditorLog.Error("Failed to create Editor Directory");
		return mission;
	}
	
	if (!MakeDirectory("$saves:/Editor/Missions/")) {
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