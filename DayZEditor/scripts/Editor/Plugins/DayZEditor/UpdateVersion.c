[WorkbenchPluginAttribute("Update Version", "Updates the mod version", "alt+5", "", {"ResourceManager", "ScriptEditor"})]
class UpdateVersionModTool: DayZTool
{
	string BatchFile;

	void UpdateVersionModTool()
	{
		BatchFile = GetWorkDriveDirectory() + "Batchfiles\\UpdateVersion.bat";
	}
	
	override void Run()
	{
		RunDayZBat(BatchFile, true);
	}
}