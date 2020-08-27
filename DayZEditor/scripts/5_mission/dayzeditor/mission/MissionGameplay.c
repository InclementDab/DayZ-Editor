
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
	
	override void OnKeyPress(int key)
	{
		// If Editor is NOT active, just do keypress
		if (!GetEditor().IsActive()) {
			super.OnKeyPress(key);
		// If Editor IS active, and DOESNT handle the key, do keypress
		} else if (!GetEditor().OnKeyPress(key)) {
			super.OnKeyPress(key);
		}		
	}
	
	override void OnUpdate(float timeslice)
	{
		if (GetEditor().IsActive()) {
			GetModuleManager().OnUpdate(timeslice);
			//GetEditor().OnUpdate(timeslice);
		} else { 
			super.OnUpdate(timeslice);
		}
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

