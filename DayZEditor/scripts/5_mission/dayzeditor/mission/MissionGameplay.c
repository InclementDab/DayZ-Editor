
modded class MissionGameplay
{

	override void OnInit()
	{
		super.OnInit();
		
		GetRPCManager().AddRPC("Editor", "StartEditor", this, SingleplayerExecutionType.Client);
		
		GetUApi().GetInputByName("UACOTModuleToggleCOT").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleButtons").ForceDisable(true);
		GetUApi().GetInputByName("UACOTTogglePlayer").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleCamera").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleESP").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleMap").ForceDisable(true);
		GetUApi().UpdateControls();
		

	}
	
	void StartEditor()
	{
		
		// Multiplayer case
		if (GetGame().IsClient() && GetGame().IsMultiplayer()) {
			EditorLog.Info("Loading Multiplayer Editor...");
			m_EditorInstance = new Editor();
		} 
		
		// Singleplayer Case
		else if (GetGame().IsServer() && !GetGame().IsMultiplayer()) {
			EditorLog.Info("Loading Singleplayer Editor...");
			m_EditorInstance = new Editor();
		}
		
		// Server case
		else {
			Print(GetGame().IsClient());
			Print(GetGame().IsServer());
			Print(GetGame().IsMultiplayer());
			EditorLog.Error("Attempted to StartEditor as Server!");
		}	
	}
		
	
	override void OnMissionStart()
	{
		super.OnMissionStart();
		if (GetGame().IsServer() && !GetGame().IsMultiplayer()) {
			StartEditor();
		}
	}
	
	
	/* Editor Pipeline */
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		
		if (IsEditor())
			m_EditorInstance.OnUpdate(timeslice);		
	}
	
	override void OnKeyPress(int key) 
	{
		super.OnKeyPress(key);
		if (IsEditor())
			m_EditorInstance.OnKeyPress(key);	
	}
	
	override void OnKeyRelease(int key) 
	{
		super.OnKeyRelease(key);
		if (IsEditor())
			m_EditorInstance.OnKeyRelease(key);	
	}
	
	
	override void OnMouseButtonPress(int button)
	{
		super.OnMouseButtonPress(button);
		if (IsEditor())
			m_EditorInstance.OnMouseButtonPress(button);	
	}
	
	override void OnMouseButtonRelease(int button)
	{
		super.OnMouseButtonRelease(button);
		if (IsEditor())
			m_EditorInstance.OnMouseButtonRelease(button);	
	}
	
	override void OnEvent(EventType eventTypeId, Param params) 
	{
		super.OnEvent(eventTypeId, params);
		if (IsEditor())
			m_EditorInstance.OnEvent(eventTypeId, params);	
	}
	
	
	
	/* Used for Offline Editor Mission Creation */
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
		FPrint(init, "	return new MissionGameplay();");
		FPrint(init, "}");
			
		CloseFile(init);
		
		return mission;
	}
}

