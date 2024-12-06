modded class DayZGame
{
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
		
		StartRandomCutscene(maps.GetRandomElement());
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
