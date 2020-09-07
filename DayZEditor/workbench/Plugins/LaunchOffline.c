[WorkbenchPluginAttribute("Launch Offline", "Launches the game in offline mode", "F5", "", {"ResourceManager", "ScriptEditor"})]
class LaunchOfflineModTool: DayZTool
{
	
	override void Run()
	{
		RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\Exit.bat", true);
		RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\LaunchClient.bat", true);
	}
}

[WorkbenchPluginAttribute("Open Log Files", "Opens logfiles in NPP", "F4", "", {"ResourceManager", "ScriptEditor"})]
class OpenLogFiles: DayZTool
{
	
	override void Run()
	{
		RunDayZBat("P:\\openlogs.bat", true);
	}
}

[WorkbenchPluginAttribute("Build Mod", "Builds current mod", "F8", "", {"ResourceManager", "ScriptEditor"})]
class BuildMod: DayZTool
{

	
	override void Run()
	{
		RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\Exit.bat", true);
		RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\ZBinarizeDeploy.bat", true);
		//RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\LaunchClient.bat", true);
	}
}

[WorkbenchPluginAttribute("Launch Server", "Launches Server", "F6", "", {"ResourceManager", "ScriptEditor"})]
class LaunchServer: DayZTool
{

	override void Run()
	{
		RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\Exit.bat", true);
		RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\ZBinarizeDeploy.bat", true);
		RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\LaunchServer.bat", true);
		RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\LaunchLocalMP.bat", true);
	}

}


