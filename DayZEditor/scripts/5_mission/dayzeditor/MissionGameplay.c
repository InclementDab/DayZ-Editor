

private ref Editor m_EditorInstance;
Editor GetEditor() { return m_EditorInstance; }

class EditorMissionGameplay: MissionGameplay
{
	
	override void OnMissionStart()
	{
		super.OnMissionStart();
		m_EditorInstance = new Editor();
	}
	
	
	override void OnInit()
	{
		super.OnInit();
	
		GetUApi().GetInputByName("UACOTModuleToggleCOT").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleButtons").ForceDisable(true);
		GetUApi().GetInputByName("UACOTTogglePlayer").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleCamera").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleESP").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleMap").ForceDisable(true);
		GetUApi().UpdateControls();
		
	}
	
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		m_EditorInstance.OnUpdate(timeslice);		
	}
	
	private string CreateEditorMission(string map_name = "ChernarusPlus")
	{
		EditorLog.Trace("EditorMissionGameplay::CreateEditorMission");
		string mission = "$saves:DayZEditor." + map_name;
		
		if (!FileExist(mission)) {
			EditorLog.Info("Editor Mission not found, creating....");
			MakeDirectory(mission);
		}
		
		FileHandle init = OpenFile(mission + "/init.c", FileMode.WRITE);
		FPrint(init, "\/\/ Returns Offline Editor Mission");
		FPrint(init, "Mission CreateCustomMission(string path)");
		FPrint(init, "{");
		FPrint(init, "	return new EditorMissionGameplay();");
		FPrint(init, "}");
			
		CloseFile(init);
		
		return mission;
	}
}

