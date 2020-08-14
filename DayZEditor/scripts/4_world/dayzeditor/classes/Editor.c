
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
	private Mission m_Mission;
	
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
		m_Mission = GetGame().GetMission();
		
		/* UI Init */
		m_UIManager = GetGame().GetUIManager();
		
		/* Player Init */
		EditorLog.Info("Initializing Player");
		m_Player = GetGame().GetPlayer();
		
		/* Camera Init */
		EditorLog.Info("Initializing Camera");
		m_Camera = GetGame().CreateObjectEx("EditorCamera", m_Player.GetPosition() + Vector(0, 2, 0), ECE_LOCAL);
		
		/* EditorUI Init */
		m_EditorUI = GetGame().GetWorkspace().CreateWidgets(EDITOR_GUI_LAYOUTS_FOLDER + "EditorUI.layout");
		
		
		/* Events Init */
		EditorEvents.OnEditorSetActive.Insert(OnEditorSetActive);

		m_Active = true;
		EditorEvents.EditorSetActive(this, m_Active);
		
	}
	
	void ~Editor()
	{
		EditorLog.Info("~Editor");
	}

	
	
	/* Event Pipeline */
	void OnUpdate(float timeslice)
	{
		/* Input Handling */
		Input input = GetGame().GetInput();
		
		if (input.LocalPress("UAUIMenu", false) || input.LocalPress("UAUIBack", false)) {
			
		}
		
		
		if (input.LocalPress("EditorToggleUI")) {
			
		} 
		
		if (input.LocalPress("EditorToggleActive")) {
			m_Active = !m_Active;
			EditorLog.Info("Editor Active: " + m_Active);
			EditorEvents.EditorSetActive(this, m_Active);
		}
		
		if (input.LocalPress("EditorToggleCursor")) {

		} 
	}
	
	void OnKeyPress(int key) {}
	void OnKeyRelease(int key) {}
	
	void OnMouseButtonPress(int button){}
	void OnMouseButtonRelease(int button){}
	
	void OnEvent(EventType event_type, Param params) 
	{
		
	}
	
	
	
	
	/* EditorEvents */
	
	// Toggles between the Editor and Player
	void OnEditorSetActive(Class context, bool state)
	{
		EditorLog.Trace("Editor::OnEditorSetActive");
		if (state) {
			m_Camera.SetActive(true);
			m_EditorUI.Show(true);
			m_Mission.GetHud().Show(false);
			
		} else {
			GetGame().SelectPlayer(GetGame().GetPlayer().GetIdentity(), GetGame().GetPlayer());
			m_EditorUI.Show(false);
			m_Mission.GetHud().Show(true);
		}
	}
}
