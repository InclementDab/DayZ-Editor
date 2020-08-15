
modded class MissionGameplay
{
	
	
	void MissionGameplay()
	{
		
	}
	
	
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
			//m_EditorInstance = new Editor();
		} 
		
		// Singleplayer Case
		else if (GetGame().IsServer() && !GetGame().IsMultiplayer()) {
			EditorLog.Info("Loading Singleplayer Editor...");
			//m_EditorInstance = new Editor();
		}
		
		// Server case
		else {
			Print(GetGame().IsClient());
			Print(GetGame().IsServer());
			Print(GetGame().IsMultiplayer());
			EditorLog.Error("Attempted to StartEditor as Server!");
		}	
	}
}

