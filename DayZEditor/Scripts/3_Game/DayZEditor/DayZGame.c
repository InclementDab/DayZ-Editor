modded class DayZGame
{	
	protected ref ScriptView m_CurrentTooltip;

	// args: string
	ref ScriptInvoker OnProgressReport;
	ref Payload_EditorLoginResponse LoginCache;

	ref ScriptInvoker Event_OnActivateMessage = new ScriptInvoker();
	ref ScriptInvoker Event_OnDeactivateMessage = new ScriptInvoker();
		
	string EditorFileToLoad;
	
	void DayZGame()
	{	
		#ifndef NO_GUI
		delete m_loading;
		m_loading = new EditorLoadingScreen(this);		
		m_loading.Show();
		#endif
		
		ReportProgress("Loading Game");
	}

	protected void OnLoginResponse(Payload_EditorLoginResponse response)
	{
		LoginCache = response;	
	}
	
	// ToolTip Control, migrated from editorhud
	void SetCurrentTooltip(notnull ScriptView current_tooltip) 
	{	
		m_CurrentTooltip = current_tooltip;
	}
	
	void ClearTooltip()
	{
		delete m_CurrentTooltip;
	}

	TooltipView CreateDelayedTooltip(Widget w, string text, TooltipPosition position, string desc = string.Empty, Symbols icon = string.Empty, int delay = 300)
	{
		TooltipView view = TooltipView.CreateOnWidget(w, text, position, desc, icon);
		DelaySetCurrentTooltip(view, w, delay);
		return view;
	}

	void DelaySetCurrentTooltip(ScriptView current_tooltip, Widget w, int delay = 300)
	{
		m_CurrentTooltip = current_tooltip;
		m_CurrentTooltip.GetLayoutRoot().Show(false);
		
		GetCallQueue(CALL_CATEGORY_GUI).Remove(VerifyCurrentTooltip);
		GetCallQueue(CALL_CATEGORY_GUI).CallLater(VerifyCurrentTooltip, delay, false, w);
	}

	protected void VerifyCurrentTooltip(Widget w)
	{
		if (GetWidgetUnderCursor() != w) {
			return;
		}
		
		if (GetMouseState(MouseState.LEFT) || GetMouseState(MouseState.RIGHT)) {
			return;
		}

		if (m_CurrentTooltip && m_CurrentTooltip.GetLayoutRoot()) {
			m_CurrentTooltip.GetLayoutRoot().Show(true);
		}
	}
	
	protected string GetUniqueEditorName()
	{
		BiosUserManager manager = g_Game.GetUserManager();

		// set user as steam user
		if (manager && manager.GetTitleInitiator()) {
			manager.SelectUserEx(manager.GetTitleInitiator());
		}

		// get steam name
		if (manager && manager.GetSelectedUser()) {
			return manager.GetSelectedUser().GetName();
		}

		return GetProfileName();
	}
	
	override bool OnInitialize()
	{
		// this will never happen, maybe requestexit -1 
		if (GetLoadState() != DayZLoadState.UNDEFINED) {
			return false;
		}
		
		// set unique player name
		string name;
		GetPlayerName(name);
		name.ToLower();

		// change name if default
		if (name == "survivor") {
			SetPlayerName(GetUniqueEditorName());
		}

		// vanilla
		ParticleList.PreloadParticles();
		RegisterProfilesOptions();

		InitNotifications();
		m_Visited = {};
		GetProfileStringList("SB_Visited", m_Visited);
		
		BiosUserManager user_manager = GetUserManager();
		if (user_manager && user_manager.GetTitleInitiator()) {
			user_manager.SelectUserEx(user_manager.GetTitleInitiator());
		}
				
		array<string> maps = {};
		for (int i = 0; i < ConfigGetChildrenCount("CfgWorlds"); i++) {
			string map_name;
			ConfigGetChildName("CfgWorlds", i, map_name);
			if (VerifyWorldOwnership(map_name) && ConfigIsExisting(string.Format("CfgWorlds %1 worldName", map_name))) {
				string text = ConfigGetTextOut(string.Format("CfgWorlds %1 ceFiles", map_name));
				if (text.Contains("DayZEditor")) {
					maps.Insert(map_name);
				}
			}
		}
		
		// For more of these to work, define CfgMissions Cutscenes ChernarusPlusIntro to the $saves dir
		string random_map = maps.GetRandomElement();
		
		SetMainMenuWorld(random_map);
		StartRandomCutscene(random_map);
		
		SetGameState(DayZGameState.MAIN_MENU);
		SetLoadState(DayZLoadState.MAIN_MENU_START);
		
		DeleteTitleScreen();
								
		// handle direct connection
		string address, port, password;
		if (GetCLIParam("connect", m_ConnectAddress)) {			
			GetCLIParam("port", port);	
			m_ConnectPort = port.ToInt();
			GetCLIParam("password", m_ConnectPassword);
											
			SetGameState(DayZGameState.CONNECTING);
			SetLoadState(DayZLoadState.CONNECT_START);
			
			Connect();			
			return true;
		}
		
		// handle sp mission launching
		string mission;
		if (GetCLIParam("mission", mission)) {			
			SetGameState(DayZGameState.IN_GAME);
			SetLoadState(DayZLoadState.MISSION_START);	
			PlayMission(mission);
			return true;
		}
		
		// idek what this is
		string party;
		if (GetCLIParam("party", party)) {
			if (user_manager) {
				user_manager.ParsePartyAsync(party);
			}
			
			return true;
		}
		
		return true;
	}
			
	void ReportProgress(string report)
	{
		if (!OnProgressReport) {
			OnProgressReport = new ScriptInvoker();
		}
		
		OnProgressReport.Invoke(report);
	}
		
	override void OnActivateMessage()
	{
		super.OnActivateMessage();

		Event_OnActivateMessage.Invoke();
	}

	override void OnDeactivateMessage()
	{
		super.OnDeactivateMessage();

		Event_OnDeactivateMessage.Invoke();
	}
}

modded class NotificationUI
{
	override void Update( float timeslice )
	{
	}
}