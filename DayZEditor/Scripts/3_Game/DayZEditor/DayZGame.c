modded class DayZGame
{
	// args: string
	ref ScriptInvoker OnProgressReport;
	
	void DayZGame()
	{
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