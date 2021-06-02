[WorkbenchPluginAttribute("Build Mod", "Builds current mod", "F8", "", {"ResourceManager", "ScriptEditor"})]
class BuildMod: DayZProjectManager
{
	override void Run()
	{
		RunDayZBatList({
			string.Format("P:\\%1\\Workbench\\Batchfiles\\Exit.bat", ModName), 
			string.Format("P:\\%1\\Workbench\\Batchfiles\\ZBinarizeDeploy.bat", ModName)
		});
	}
}

[WorkbenchPluginAttribute("Build Mod", "Builds current mod", "Ctrl+F8", "", {"ResourceManager", "ScriptEditor"})]
class BuildModAndLaunch: DayZProjectManager
{
	override void Run()
	{
		RunDayZBatList({
			string.Format("P:\\%1\\Workbench\\Batchfiles\\Exit.bat", ModName), 
			string.Format("P:\\%1\\Workbench\\Batchfiles\\ZBinarizeDeploy.bat", ModName), 
			string.Format("P:\\%1\\Workbench\\Batchfiles\\LaunchOffline.bat", ModName)
		});
	}
}

[WorkbenchPluginAttribute("Launch Server", "Launches Server", "F6", "", {"ResourceManager", "ScriptEditor"})]
class LaunchServer: DayZProjectManager
{
	override void Run()
	{
		RunDayZBat(string.Format("P:\\%1\\Workbench\\Batchfiles\\Exit.bat", ModName), true);
		RunDayZBat(string.Format("P:\\%1\\Workbench\\Batchfiles\\LaunchServer.bat", ModName));
		RunDayZBat(string.Format("P:\\%1\\Workbench\\Batchfiles\\LaunchLocalMP.bat", ModName));
	}
}

[WorkbenchPluginAttribute("Launch and Build Server", "Launches Server", "Ctrl+F6", "", {"ResourceManager", "ScriptEditor"})]
class LaunchBuildServer: DayZProjectManager
{
	override void Run()
	{
		RunDayZBat(string.Format("P:\\%1\\Workbench\\Batchfiles\\Exit.bat", ModName), true);
		RunDayZBat(string.Format("P:\\%1\\Workbench\\Batchfiles\\ZBinarizeDeploy.bat", ModName), true);
		RunDayZBat(string.Format("P:\\%1\\Workbench\\Batchfiles\\LaunchServer.bat", ModName));
		RunDayZBat(string.Format("P:\\%1\\Workbench\\Batchfiles\\LaunchLocalMP.bat", ModName));
	}
}
