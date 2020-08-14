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
	string BatchFile;

	void OpenLogFiles()
	{
		BatchFile = "P:\\openlogs.bat";
	}
	
	override void Run()
	{
		BetterRunDayZBat(BatchFile, "", true);
	}
}

[WorkbenchPluginAttribute("Build Mod", "Builds current mod", "F8", "", {"ResourceManager", "ScriptEditor"})]
class BuildMod: DayZTool
{
	string BatchFile;

	void BuildMod()
	{
		RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\Exit.bat", true);
		RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\ZBinarizeDeploy.bat", true);
		//RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\LaunchClient.bat", true);
	}
	
	override void Run()
	{
		BetterRunDayZBat(BatchFile, "", true);
	}
}

[WorkbenchPluginAttribute("Launch Server", "Launches Server", "F9", "", {"ResourceManager", "ScriptEditor"})]
class LaunchServer: DayZTool
{
	string BatchFile;

	void LaunchServer()
	{
		RunDayZBat("P:\\DayZ_Server\\server.bat", true);
	}
	
	override void Run()
	{
		BetterRunDayZBat(BatchFile, "", true);
	}
}


void BetterRunDayZBat(string filepath, string params = "", bool wait = false)
{
	
	if (filepath.Length() < 2) return;
	
	filepath.Replace("\\", "/");
	
	if (filepath[1] != ":")
	{
		string cwd;
		Workbench.GetCwd(cwd);
		filepath = cwd + "/" + filepath;
	}
	
	int index = filepath.IndexOf("/");
	int last_index = index;
	
	while(index != -1)
	{
		last_index = index;
		index = filepath.IndexOfFrom(last_index + 1, "/");
	}
	
	if (last_index == -1) return;
	
	string path = filepath.Substring(0, last_index);
	string bat = filepath.Substring(last_index + 1, filepath.Length() - last_index - 1);
	
	string final = "cmd /c \"chdir /D " + path + " & call " + bat + "\"";
	Print(final);
	Workbench.RunCmd(final, wait);
}