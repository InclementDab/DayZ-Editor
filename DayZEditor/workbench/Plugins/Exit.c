[WorkbenchPluginAttribute("Exit Game", "Exits the game", "alt+2", "", {"ResourceManager", "ScriptEditor"})]
class ExitModTool: DayZTool
{
	string BatchFile;

	void ExitModTool()
	{
		BatchFile = GetWorkDriveDirectory() + "Batchfiles\\Exit.bat";
	}
	
	override void Run()
	{
		RunDayZBat(BatchFile, true);
	}
}