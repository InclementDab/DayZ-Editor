
private ref Editor m_EditorInstance;
Editor GetEditor() 
{ 	
	//if (m_EditorInstance == null)
	//	m_EditorInstance = new Editor();
	return m_EditorInstance;
}

bool IsEditor() { return m_EditorInstance != null; }


// Editor should be run on the client that wants to be editor
class Editor: JMModuleBase
{
	/* Private Members 
	*	Keep these in the same order as in the constructor
	*/
	private Mission m_Mission;
	
	private UIManager m_UIManager;
	private PlayerBase m_Player;
	private PlayerIdentity m_PlayerIdentity;
	private EditorCamera m_Camera;
	
	private bool m_Active;
	private Widget m_EditorUI;
	
	
	// Public Members
	void Editor()
	{
		EditorLog.Info("Editor");			
	}
	
	void ~Editor()
	{
		EditorLog.Info("~Editor");
	}
	
	override void OnInit()
	{
		EditorLog.Trace("Editor::OnInit");
		
		
		/* UI Init */
		m_UIManager = GetGame().GetUIManager();
				
		/* EditorUI Init */
		m_EditorUI = GetGame().GetWorkspace().CreateWidgets(EDITOR_GUI_LAYOUTS_FOLDER + "EditorUI.layout");
		
		RegisterBinding(new JMModuleBinding("OnEditorToggleActive", "EditorToggleActive"));
		
	}
	
	
	
	override void OnMissionStart()
	{
		EditorLog.Trace("Editor::OnMissionStart");
		EditorLog.Info("Loading Singleplayer Editor...");
		m_Mission = GetGame().GetMission();
		if (GetGame().IsServer() && !GetGame().IsMultiplayer()) {
			m_EditorInstance = this;
			m_Active = true;
		}
		
		SetActive(m_Active);	
	}
	
	override void OnMissionLoaded()
	{
		EditorLog.Trace("Editor::OnMissionLoaded");
		
		if (IsMissionClient()) {
			
			// Player Init
			EditorLog.Info("Initializing Player");
			m_Player = GetGame().GetPlayer();
			m_PlayerIdentity = m_Player.GetIdentity();
			
			// Camera Init
			EditorLog.Info("Initializing Camera");
			m_Camera = GetGame().CreateObjectEx("EditorCamera", m_Player.GetPosition() + Vector(0, 2, 0), ECE_LOCAL);
		}
	}
	
	
	private void OnEditorToggleActive(UAInput input)
	{
		if (!input.LocalPress()) return;
		EditorLog.Trace("Editor::OnEditorToggleActive");
		m_Active = !m_Active;
		SetActive(m_Active);
	}
	
	
	void SetActive(bool active)
	{
		m_Camera.SetActive(m_Active);
		m_EditorUI.Show(m_Active);
		m_Mission.GetHud().Show(!m_Active);
		
		if (!m_Active)
			GetGame().SelectPlayer(m_PlayerIdentity, m_Player);
	}
	
	
	
	override bool IsServer()
	{
		return false;
	}

}
