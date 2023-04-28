modded class DayZGame
{
	// args: string
	ref ScriptInvoker OnProgressReport;
		
	void DayZGame()
	{	
		#ifndef NO_GUI
		delete m_loading;
		m_loading = new EditorLoadingScreen(this);		
		m_loading.Show();
		#endif
		
		ReportProgress("Loading Game");
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
		GetGame().GetUIManager().ShowDialog("Editor Error!", "You cannot run the DayZ Editor in a multiplayer environment. Launch the tool via the \"Open Editor\" button on the main menu!", 1, DBT_OK, DBB_OK, DMT_EXCLAMATION, null);
	}
}