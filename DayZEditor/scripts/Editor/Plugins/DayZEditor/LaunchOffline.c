
[WorkbenchPluginAttribute("Launch Offline", "Launches the game in offline mode", "F5", "", {"ResourceManager", "ScriptEditor"})]
class LaunchOfflineModTool: DayZProjectManager
{
	override void Run()
	{
		RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\Exit.bat", true);
		RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\LaunchOffline.bat", true);
	}
}

[WorkbenchPluginAttribute("Open Log Files", "Opens logfiles in NPP", "F4", "", {"ResourceManager", "ScriptEditor"})]
class OpenLogFiles: DayZProjectManager
{
	
	override void Run()
	{
		RunDayZBat("P:\\openlogs.bat", true);
	}
}

[WorkbenchPluginAttribute("Build Mod", "Builds current mod", "F8", "", {"ResourceManager", "ScriptEditor"})]
class BuildMod: DayZProjectManager
{
	override void Run()
	{
		RunDayZBatList({"P:\\DayZEditor\\workbench\\Batchfiles\\Exit.bat", "P:\\DayZEditor\\workbench\\Batchfiles\\ZBinarizeDeploy.bat"});
	}
}

[WorkbenchPluginAttribute("Build Mod", "Builds current mod", "Ctrl+F8", "", {"ResourceManager", "ScriptEditor"})]
class BuildModAndLaunch: DayZProjectManager
{
	override void Run()
	{
		RunDayZBatList({"P:\\DayZEditor\\workbench\\Batchfiles\\Exit.bat", "P:\\DayZEditor\\workbench\\Batchfiles\\ZBinarizeDeploy.bat", "P:\\DayZEditor\\workbench\\Batchfiles\\LaunchOffline.bat"});
	}
}

[WorkbenchPluginAttribute("Launch Server", "Launches Server", "Ctrl+F6", "", {"ResourceManager", "ScriptEditor"})]
class LaunchServer: DayZProjectManager
{

	override void Run()
	{
		RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\Exit.bat", true);
		RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\ZBinarizeDeploy.bat", true);
		RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\LaunchServer.bat");
		RunDayZBat("P:\\DayZEditor\\workbench\\Batchfiles\\LaunchLocalMP.bat");
	}
}


[WorkbenchPluginAttribute("Edit Project", "Edit Project", "", "", {"ResourceManager", "ScriptEditor"})]
class DayZProjectManager: DayZTool
{
	[Attribute("", "editbox")]
	string Directory;
	
		
	override void Run()
	{
		Workbench.ScriptDialog("Edit Project Settings", "", this);
	}
	
	override void RunDayZBat(string filepath, bool wait = false)
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
		
		Print("cmd /c \"chdir /D " + path + " & call " + bat + "\"");
		Workbench.RunCmd("cmd /c \"chdir /D " + path + " & call " + bat + "\"", wait);
	}
	
	
	void RunDayZBatList(array<string> filepaths)
	{
		foreach (string file: filepaths) {
			RunDayZBat(file, true);
		}
	}
}


