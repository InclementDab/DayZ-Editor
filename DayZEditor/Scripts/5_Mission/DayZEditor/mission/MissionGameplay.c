
modded class MissionGameplay
{	
	protected ref EditorMainMenu m_PauseMenu;

	protected ref EditorClientManager m_EditorClientManager;

	ref map<int, Object> Cameras = new map<int, Object>();
	ref map<int, ref PlayerCameraData> LastCameraData = new map<int, ref PlayerCameraData>();
	ref map<int, ref PlayerCameraData> CameraData = new map<int, ref PlayerCameraData>();
	
	ref map<int, ref EditorCameraMarker> CameraMarkers = new map<int, ref EditorCameraMarker>();
	protected bool m_AutoInitializeEditor;
		
	void MissionGameplay()
	{
		m_AutoInitializeEditor = CreateEditorOnStart();
    	DayZGame.Event_OnRPC.Insert(OnERPC);
	}
	
	override void OnInit()
	{
		super.OnInit();
		m_EditorClientManager = new EditorClientManager();
	}

	override void OnKeyPress(int key)
	{
		if (GetEditor()) {
			if (GetEditor().OnKeyPress(key)) {
				return;
			}
		}
		
		super.OnKeyPress(key);
	}

	override void OnKeyRelease(int key)
	{
		if (GetEditor()) {
			if (GetEditor().OnKeyRelease(key)) {
				return;
			}
		}
		
		super.OnKeyRelease(key);
	}

	override void OnMouseButtonRelease(int button)
	{
		if (GetEditor()) {
			if (GetEditor().OnMouseRelease(button)) {
				return;
			}
		}
		
		super.OnMouseButtonRelease(button);
	}

	override void OnUpdate(float timeslice)
	{
		if (m_EditorClientManager)
			m_EditorClientManager.OnUpdate(timeslice);

		if (GetEditor()) {
			GetEditor().Update(timeslice);
		} else {
			super.OnUpdate(timeslice);
		}
	}
	
	/*
	override void ShowInventory()
	{
		GetGame().GetPlayer().GetHumanInventory().UnlockInventory(LOCK_FROM_SCRIPT);
		super.ShowInventory();
		m_Hud.InventoryShown = true;
	}
	
	override void HideInventory()
	{
		super.HideInventory();
		m_Hud.InventoryShown = false;
	}*/
	
	override void OnMissionFinish()
	{
		if (g_Editor)
		{
			delete g_Editor;
			g_Editor = null;
		}
		super.OnMissionFinish();
	}
	
	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();
		
		// Server can handle itself
		if (GetGame().IsMultiplayer()) {
			return;
		}

		// In the event we handle the creation on our own
		if (m_AutoInitializeEditor) {
			vector center_pos = Editor.GetMapCenterPosition();
			vector start_pos = Editor.GetSafeStartPosition(center_pos[0], center_pos[2], 3500);
			PlayerBase player = Editor.CreateDefaultCharacter(GetGame().CreateRandomPlayer(), start_pos);
			if (!player) {
				Error("Player was not created, exiting");
				return;
			}
	
			// Make sure to select player immediately so they can be controlled
			GetGame().SelectPlayer(null, player);
	
			g_Editor = new Editor(player);
			g_Editor.SetActive(true);
		}
	}
	
	bool CreateEditorOnStart()
	{
		return true;
	}
			
	override bool IsPaused()
	{
		if (!m_AutoInitializeEditor) {
			return super.IsPaused();
		}
		
		return m_PauseMenu != null;
	}
	
	override void Pause()
	{
		if (!m_AutoInitializeEditor) {
			super.Pause();
			return;
		}
		
		if (IsPaused() || m_PauseMenu)
		{
			return;
		}

		m_PauseQueued = true;

		if (g_Game.GetGameState() != DayZGameState.IN_GAME) {
			return;
		}
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player && !player.IsPlayerLoaded() || IsPlayerRespawning()) {
			return;
		}
		
		CloseAllMenus();
		
		// open ingame menu
		m_PauseMenu = new EditorMainMenu();

		AddActiveInputExcludes({"menu"});
		AddActiveInputRestriction(EInputRestrictors.INVENTORY);

		m_PauseQueued = false;
	}
	
	override void Continue()
	{
		if (!m_AutoInitializeEditor) {
			super.Continue();
			return;
		}
		
		//GetGame().GetUIManager().Back();
		
		RemoveActiveInputExcludes({"menu"},true);
		RemoveActiveInputRestriction(EInputRestrictors.INVENTORY);
		delete m_PauseMenu;
			
		if (GetEditor() && GetEditor().IsActive())	{
			GetEditor().GetEditorHud().Show(true);
		}
	}

	void OnERPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		if (m_EditorClientManager)
			m_EditorClientManager.OnRPC(sender, target, rpc_type, ctx);
	}
}