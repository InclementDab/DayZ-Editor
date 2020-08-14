
private ref Editor m_EditorInstance;
Editor GetEditor() 
{ 	
	if (m_EditorInstance == null)
		m_EditorInstance = new Editor();
	return m_EditorInstance;
}

bool IsEditor() { return m_EditorInstance != null; }


// Editor should be run on the client that wants to be editor
class Editor
{
	// Private Members
	private EditorCamera m_Camera;
	private PlayerBase m_Player;
	
	// Public Members
	
	
	
	
	void Editor()
	{
		EditorLog.Info("Editor");	
		
		// Player Init
		EditorLog.Info("Initializing Player");
		m_Player = GetGame().GetPlayer();
		EditorLog.Info(m_Player);
		
		// Camera Init
		EditorLog.Info("Initializing Camera");
		vector pos = m_Player.GetPosition();
		pos[1] = pos[1] + 3;
		m_Camera = GetGame().CreateObjectEx("EditorCamera", pos, ECE_LOCAL);
		m_Camera.SetActive(true);
		
	}
	
	void ~Editor()
	{
		EditorLog.Info("~Editor");
	}

	
	
	
	void OnUpdate(float timeslice)
	{
		
		Input input = GetGame().GetInput();
		if (input.LocalPress("UAUIMenu", false) || input.LocalPress("UAUIBack", false)) {
			
		}
		
		
		if (input.LocalPress("EditorToggleUI")) {
			
		} 
		
		if (input.LocalPress("EditorToggleCursor")) {

		} 
	}
}
