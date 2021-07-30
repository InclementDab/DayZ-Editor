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
}