


// Editor should be run on the client that wants to be editor
class Editor
{
	// Private Members
	private EditorCamera m_Camera;
	private PlayerBase m_Player;
	
	// Public Members
	
	
	
	
	void Editor()
	{
		EditorLog.Trace("Editor");	
		
		// Player Init
		EditorLog.Info("Initializing Player");
		m_Player = GetGame().GetPlayer();
		EditorLog.Info(m_Player);
		
		// Camera Init
		EditorLog.Info("Initializing Camera");
		m_Camera = GetGame().CreateObjectEx("EditorCamera", vector.Zero, ECE_NONE);
		m_Camera.SetActive(true);
		
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
