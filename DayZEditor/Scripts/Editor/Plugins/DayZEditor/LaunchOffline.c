
[WorkbenchPluginAttribute("Launch Offline", "Launches the game in offline mode", "F5", "", {"ResourceManager", "ScriptEditor"})]
class LaunchOfflineModTool: DayZProjectManager
{
	override void Run()
	{
		RunDayZBat(string.Format("P:\\%1\\Workbench\\Batchfiles\\Exit.bat", ModName), true);
		RunDayZBat(string.Format("P:\\%1\\Workbench\\Batchfiles\\LaunchOffline.bat", ModName), true);
	}
}



