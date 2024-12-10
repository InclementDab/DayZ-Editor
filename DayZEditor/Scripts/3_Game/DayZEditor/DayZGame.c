modded class DayZGame
{
	protected ref ScriptView m_CurrentTooltip;
	
	// args: string
	ref ScriptInvoker OnProgressReport;

	ref ScriptInvoker Event_OnActivateMessage = new ScriptInvoker();
	ref ScriptInvoker Event_OnDeactivateMessage = new ScriptInvoker();
		
	void DayZGame()
	{	
		#ifndef NO_GUI
		delete m_loading;
		m_loading = new EditorLoadingScreen(this);		
		m_loading.Show();
		#endif
		
		ReportProgress("Loading Game");
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

		if (m_CurrentTooltip && m_CurrentTooltip.GetLayoutRoot()) {
			m_CurrentTooltip.GetLayoutRoot().Show(true);
		}
	}
	
	override void MainMenuLaunch()
	{
#ifdef PLATFORM_WINDOWS
		BiosUserManager user_manager = GetUserManager();
		if (user_manager)
		{
			if (user_manager.GetTitleInitiator())
			{
				user_manager.SelectUserEx(user_manager.GetTitleInitiator());
			}
		}
#endif
		
		SetGameState(DayZGameState.MAIN_MENU);
		SetLoadState(DayZLoadState.MAIN_MENU_START);	
		
		array<string> maps = {};
		for (int i = 0; i < ConfigGetChildrenCount("CfgWorlds"); i++) {
			string name;
			ConfigGetChildName("CfgWorlds", i, name);
			if (VerifyWorldOwnership(name) && ConfigIsExisting(string.Format("CfgWorlds %1 worldName", name))) {
				maps.Insert(name);
			}
		}
		
		string random_map = maps.GetRandomElement();
		string mission_target = SystemPath.Saves(string.Format("EditorMainMenu.%1", random_map));
		DeleteFile(mission_target);
		MakeDirectory(mission_target);
		CopyFile("DayZEditor\\Scripts\\Data\\Defaults\\MainMenuMission\\init.c", SystemPath.Combine(mission_target, "init.c"));
		mission_target.Replace("/", "\\");
		mission_target.Replace(":\\", ":");
		PlayMission(mission_target);
		
		DeleteTitleScreen();
	}
		
	void ReportProgress(string report)
	{
		if (!OnProgressReport) {
			OnProgressReport = new ScriptInvoker();
		}
		
		OnProgressReport.Invoke(report);
	}
		
	override void ConnectFromCLI()
	{
		Error("You cannot run the DayZ Editor in a multiplayer environment. Launch the tool via the \"Open Editor\" button on the main menu!");
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
