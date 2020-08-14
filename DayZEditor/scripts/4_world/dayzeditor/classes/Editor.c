
private ref EditorClientModule m_EditorInstance;
EditorClientModule GetEditor() 
{ 	
	//if (m_EditorInstance == null)
	//	m_EditorInstance = new Editor();
	return m_EditorInstance;
}

bool IsEditor() { return m_EditorInstance != null; }


enum EditorClientModuleRPC
{
	INVALID = 36114,
	EDITOR_CLIENT_UPDATE,
	EDITOR_CLIENT_CREATED,
	EDITOR_CLIENT_DESTROYED,
	COUNT
};


class EditorClientModule: JMModuleBase
{
	/* Private Members */
	private Mission m_Mission;
	
	private UIManager m_UIManager;
	private EditorCamera m_Camera;
	EditorCamera GetCamera() { return m_Camera; }
	
	private ref map<int, Object> m_EditorClientModels;
	
	
	private PlayerBase m_Player;
		
	private ref EditorObjectDataSet	m_SessionCache;
	EditorObjectDataSet GetSessionCache() { return m_SessionCache; }
	
	private bool m_Active = false;
	private Widget m_EditorUI;
	
	
	// Public Members
	void EditorClientModule()
	{
		EditorLog.Info("Editor");
		m_EditorInstance = this;	
		
	}
	
	void ~EditorClientModule()
	{
		EditorLog.Info("~Editor");
	}
	
	override void OnInit()
	{
		EditorLog.Trace("Editor::OnInit");
		m_SessionCache = new EditorObjectDataSet();
		m_EditorClientModels = new map<int, Object>();
		
		/* UI Init */
		m_UIManager = GetGame().GetUIManager();
				
		/* EditorUI Init */
		m_EditorUI = GetGame().GetWorkspace().CreateWidgets(EDITOR_GUI_LAYOUTS_FOLDER + "EditorUI.layout");
		m_EditorUI.Show(false);
		
		RegisterBinding(new JMModuleBinding("OnEditorToggleActive", "EditorToggleActive"));
		
	}
	
	override void OnUpdate(float timeslice)
	{
		if (m_Camera != null) {
			ScriptRPC update_rpc = new ScriptRPC();
			update_rpc.Write(m_Camera.GetPosition());
			update_rpc.Write(m_Camera.GetOrientation());
			update_rpc.Send(null, EditorServerModuleRPC.EDITOR_SERVER_UPDATE, true);
		}
	}
	
	
	override void OnMPSessionPlayerReady()
	{
		
				
		
	}
	
	override void OnInvokeConnect( PlayerBase player, PlayerIdentity identity )
	{
		EditorLog.Trace("Editor::OnInvokeConnect");		
		m_Player = player;
		EditorLog.Debug(m_Player);
	}
	
	override void OnMissionStart()
	{
		EditorLog.Trace("Editor::OnMissionStart");
		m_Mission = GetGame().GetMission();
		
		// Only runs in Singleplayer
		if (GetGame().IsServer() && !GetGame().IsMultiplayer()) {
			EditorLog.Info("Loading Singleplayer Editor...");
			m_Player = GetGame().GetPlayer();
			m_Active = true;
			SetActive(m_Active);
		}
		
		
	}
	
	override void OnMissionLoaded()
	{
		EditorLog.Trace("Editor::OnMissionLoaded");
		

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
		if (m_Camera == null) {
			// Camera Init
			EditorLog.Info("Initializing Camera");
			m_Camera = GetGame().CreateObjectEx("EditorCamera", m_Player.GetPosition() + Vector(0, 10, 0), ECE_LOCAL);
		}
		
		if (m_Player == null) {
			m_Player = GetGame().GetPlayer();
		}
		
		ScriptRPC rpc = new ScriptRPC();
		if (active) {
			rpc.Write(this);
			rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_CREATE, true);
		} else {
			rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_DESTROY, true);
		}
		
		m_Camera.SetActive(active);
		m_EditorUI.Show(active);
		m_Mission.GetHud().Show(!active);
		
		
		if (!active)
			GetGame().SelectPlayer(m_Player.GetIdentity(), m_Player);
	}
	
	
	
	override int GetRPCMin()
	{
		return EditorClientModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return EditorClientModuleRPC.COUNT;
	}
	
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx)
	{
		switch (rpc_type) {
			
			case EditorClientModuleRPC.EDITOR_CLIENT_UPDATE: {
				//EditorLog.Debug("Editor::EDITOR_CLIENT_UPDATE");
				int id;
				vector pos, ori;
				ctx.Read(id);
				ctx.Read(pos);
				ctx.Read(ori);
				
				Object cam = m_EditorClientModels.Get(id);
				cam.SetPosition(pos);
				cam.SetOrientation(ori);
			
				
				break;
			}
			
			case EditorClientModuleRPC.EDITOR_CLIENT_CREATED: {
				EditorLog.Debug("Editor::EDITOR_CLIENT_CREATE");
				ctx.Read(id);
				m_EditorClientModels.Set(id, GetGame().CreateObjectEx("DSLRCamera", vector.Zero, ECE_LOCAL));
				Print(m_EditorClientModels.Count());
				break;
			}
			
			case EditorClientModuleRPC.EDITOR_CLIENT_DESTROYED: {
				EditorLog.Debug("Editor::EDITOR_CLIENT_DESTROY");
				ctx.Read(id);
				m_EditorClientModels.Remove(id);
				break;
			}
			

			
			
		}
	}
	

	
	override bool IsServer() { return false; }

}
