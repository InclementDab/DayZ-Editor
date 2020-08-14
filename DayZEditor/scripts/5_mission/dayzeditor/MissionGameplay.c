

private ref Editor m_EditorInstance;
Editor GetEditor() 
{ 	
	if (m_EditorInstance == null)
		m_EditorInstance = new Editor();
	return m_EditorInstance;
}


class EditorMissionServer: MissionServer
{
	
	void EditorMissionServer()
	{
		GetRPCManager().AddRPC("Editor", "CreateOnPlayer", this, SingleplayerExecutionType.Client);
	}
	
	static void CreateEditorOnPlayer(PlayerIdentity player)
	{
		GetRPCManager().SendRPC("Editor", "CreateOnPlayer", null, true, player); 
	}
	
	private void CreateOnPlayer()
	{
		if (!GetGame().IsClient() || GetGame().IsServer()) {
			EditorLog.Error("Attempted to CreateOnPlayer as Server!");
			return;
		}
		
		m_EditorInstance = GetEditor();
	}
	
}


class EditorMissionGameplay: MissionGameplay
{

	override void OnInit()
	{
		super.OnInit();
		
		GetRPCManager().AddRPC("Editor", "CreateOnPlayer", this, SingleplayerExecutionType.Client);
		
		GetUApi().GetInputByName("UACOTModuleToggleCOT").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleButtons").ForceDisable(true);
		GetUApi().GetInputByName("UACOTTogglePlayer").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleCamera").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleESP").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleMap").ForceDisable(true);
		GetUApi().UpdateControls();
		
	}
	
	private void CreateOnPlayer()
	{
		Print(GetGame().IsClient());
		Print(GetGame().IsServer());
		Print(GetGame().IsMultiplayer());
		
		if (!GetGame().IsClient() || GetGame().IsServer()) {
			EditorLog.Error("Attempted to CreateOnPlayer as Server!");
		}
		
		m_EditorInstance = new Editor();
	
	}
	
	
	override void OnMissionStart()
	{
		super.OnMissionStart();
		//m_EditorInstance = GetEditor();
	}
	
	
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		//m_EditorInstance.OnUpdate(timeslice);		
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

