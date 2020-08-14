
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
	/* Private Members 
	*	Keep these in the same order as in the constructor
	*/
	private MissionGameplay m_MissionGameplay;
	private UIManager m_UIManager;
	private PlayerBase m_Player;
	private EditorCamera m_Camera;
	
	private bool m_Active;
	
	private Widget m_EditorUI;
	
	// Public Members
	
	
	
	
	void Editor()
	{
		EditorLog.Info("Editor");	
		
		/* Mission Init */
		m_MissionGameplay = GetGame().GetMission();
		
		/* UI Init */
		m_UIManager = GetGame().GetUIManager();
		
		/* Player Init */
		EditorLog.Info("Initializing Player");
		m_Player = GetGame().GetPlayer();
		
		/* Camera Init */
		EditorLog.Info("Initializing Camera");
		m_Camera = GetGame().CreateObjectEx("EditorCamera", m_Player.GetPosition() + Vector(0, 2, 0), ECE_LOCAL);
		
		
		SetActive(true);
		
	}
	
	void ~Editor()
	{
		EditorLog.Info("~Editor");
	}

	
	
	
	void OnUpdate(float timeslice)
	{
		/* Input Handling */
		Input input = GetGame().GetInput();
		
		if (input.LocalPress("UAUIMenu", false) || input.LocalPress("UAUIBack", false)) {
			
		}
		
		
		if (input.LocalPress("EditorToggleUI")) {
			
		} 
		
		if (input.LocalPress("EditorToggleActive")) {
			SetActive(!m_Active);
		}
		
		if (input.LocalPress("EditorToggleCursor")) {

		} 
	}
	
	// Toggles between the Editor and Player
	private void SetActive(bool active)
	{
		EditorLog.Trace("Editor::SetActive");
		
		m_Active = active;
		if (m_Active) {
			m_Camera.SetActive(true);
			m_MissionGameplay.PlayerControlDisable(INPUT_EXCLUDE_ALL);
			m_MissionGameplay.m_HudRootWidget.Show(false);
			m_EditorUI.Show(true);
			
		} else {
			GetGame().SelectPlayer(GetGame().GetPlayer().GetIdentity(), GetGame().GetPlayer());
			m_MissionGameplay.PlayerControlEnable(true);
			m_MissionGameplay.m_HudRootWidget.Show(true);
			m_EditorUI.Show(false);

		}
	}
}
