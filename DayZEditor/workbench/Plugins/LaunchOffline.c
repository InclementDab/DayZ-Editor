[WorkbenchPluginAttribute("Launch Offline", "Launches the game in offline mode", "F5", "", {"ResourceManager", "ScriptEditor"})]
class LaunchOfflineModTool: DayZTool
{
	string BatchFile;

	void LaunchOfflineModTool()
	{
		BatchFile = "P:\\client.bat";
	}
	
	override void Run()
	{
		RunDayZBat(BatchFile, true);
	}
}

[WorkbenchPluginAttribute("Open Log Files", "Opens logfiles in NPP", "F4", "", {"ResourceManager", "ScriptEditor"})]
class OpenLogFiles: DayZTool
{
	string BatchFile;

	void OpenLogFiles()
	{
		BatchFile = "P:\\openlogs.bat";
	}
	
	override void Run()
	{
		RunDayZBat(BatchFile, true);
	}
}