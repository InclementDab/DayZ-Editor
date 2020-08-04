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