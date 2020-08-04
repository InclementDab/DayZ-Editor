[WorkbenchPluginAttribute("Launch Local MP", "Launches the game and server, automatically connects", "alt+4", "", {"ResourceManager", "ScriptEditor"})]
class LaunchLocalMPModTool: DayZTool
{
	string BatchFile;

	void LaunchLocalMPModTool()
	{
		BatchFile = GetWorkDriveDirectory() + "Batchfiles\\LaunchLocalMP.bat";
	}
	
	override void Run()
	{
		RunDayZBat(BatchFile, true);
	}
}