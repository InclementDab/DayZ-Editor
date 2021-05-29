
[WorkbenchPluginAttribute("Edit Project", "Edit Project", "", "", {"ResourceManager", "ScriptEditor"})]
class DayZProjectManager: DayZTool
{
	string ModName = "DayZEditor";
		
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
