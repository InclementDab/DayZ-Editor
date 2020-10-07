[WorkbenchPluginAttribute("Deploy Mod", "Deploys the active loaded mod", "alt+1", "", {"ResourceManager", "ScriptEditor"})]
class DeployModTool: DayZTool
{
	string BatchFile;

	void DeployModTool()
	{
		BatchFile = GetWorkDriveDirectory() + "Batchfiles\\Deploy.bat";
	}
	
	override void Run()
	{
		RunDayZBat(BatchFile, true);
	}
}