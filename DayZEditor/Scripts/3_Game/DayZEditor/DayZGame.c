modded class DayZGame
{
	protected ref ScriptView m_CurrentTooltip;
	
#ifdef DIAG_DEVELOPER
	static const string WEB_API_ENDPOINT = "http:\/\/127.0.0.1:5000\/";
#else
	static const string WEB_API_ENDPOINT = "http:\/\/us-nyc02.pylex.xyz:8612\/";
#endif
	
	// args: string
	ref ScriptInvoker OnProgressReport;

	ref ScriptInvoker Event_OnActivateMessage = new ScriptInvoker();
	ref ScriptInvoker Event_OnDeactivateMessage = new ScriptInvoker();
		
	string EditorFileToLoad;
	
	ref Payload_EditorLoginResponse LoginCache;
	
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
		string mission_directory = SystemPath.Saves("EditorCache");
		MakeDirectory(mission_directory);

		string mission_target = SystemPath.Combine(mission_directory, string.Format("EditorMainMenu.%1", random_map));
		string mission_ce_folder = SystemPath.Combine(mission_target, "db");

		DeleteFile(mission_target);

		MakeDirectory(mission_target);
		MakeDirectory(mission_ce_folder);

		string init_file = SystemPath.Combine(mission_target, "init.c");
		string econ_file = SystemPath.Combine(mission_ce_folder, "economy.xml");
		CopyFile("DayZEditor\\Scripts\\Data\\Defaults\\MainMenuMission\\init.c", init_file);
		CopyFile("DayZEditor\\Scripts\\Data\\Defaults\\MainMenuMission\\economy.xml", econ_file);
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

	
static bool IsEditorCLEForceDisabled()
{
	Print(FileExist(SystemPath.Profile("π")));
	if (FileExist(SystemPath.Profile("π"))) {
		return true;
	}
	
	array<string> rpt = Directory.EnumerateFiles("$profile:", "*.RPT");
	rpt.Debug();
	string highest_file;
	string second_highest;
	int highest = -1;
	foreach (string file_name: rpt) {
		 array<string> log_file_name_split_major = {};
	    // date[2]
	    // time[3]
	    file_name.Split("_", log_file_name_split_major);
	
	    array<string> log_file_name_split_minor = {};
	    // getting date
	    log_file_name_split_major[2].Split("-", log_file_name_split_minor);
	    int year = log_file_name_split_minor[0].ToInt();
	    int month = log_file_name_split_minor[1].ToInt();
	    int day = log_file_name_split_minor[2].ToInt();
	
	    log_file_name_split_minor.Clear();
	    log_file_name_split_major[3].Split("-", log_file_name_split_minor);
	    int hour = log_file_name_split_minor[0].ToInt();
	    int minute = log_file_name_split_minor[1].ToInt();
	    int second = log_file_name_split_minor[2].ToInt();
	    DateTime file_time_stamp = DateTime.Create(year, month, day, hour, minute, second);
		
		if (file_time_stamp > highest) {
			if (highest_file) {
				second_highest = highest_file;
			}
			
	        highest_file = file_name;		
	        highest = file_time_stamp;
	    }
	}
	
	Print(second_highest);
	FileHandle handle = OpenFile(second_highest, FileMode.READ);
	for (int i = 0; i < 8; i++) {
		string x;
		ReadFile(handle, x, 4096);
		if (x.Contains("Mission script didn't initialize Hive, player connect will stay disabled!")) {
			CloseFile(handle);
			File.Create(SystemPath.Profile("π"));
			return true;
		}
	}
	
	CloseFile(handle);
	return false;
}
