[WorkbenchPluginAttribute("New File", "Creates new file", "Ctrl+N", "", {"ScriptEditor"})]
class CreateNewFilePlugin: WorkbenchPlugin
{
	[Attribute("", "editbox", "File Name")]
	string FileName;
	
	string current_file;
	string absolute_path;
	
	override void Run() 
	{
		ScriptEditor module = Workbench.GetModule("ScriptEditor");
		if (!module) {
			PluginErrorMessage.Show("Module was null!");
			return;
		}

		if (module.GetCurrentFile(current_file) && Workbench.GetAbsolutePath(current_file, absolute_path)) {
			
			absolute_path.Replace("\\", "/");		
			int index = absolute_path.IndexOf("/");
			int last_index = index;
			
			while (index != -1) {
				last_index = index;
				index = absolute_path.IndexOfFrom(last_index + 1, "/");
			}
			
			absolute_path = absolute_path.Substring(0, last_index);
			
			Workbench.ScriptDialog("Create new file...", "File Name", this);
		}	
	}
	
	[ButtonAttribute("Create")]
	void Create()
	{
		if (FileName.Length() == 0) {
			PluginErrorMessage.Show("File name is empty!");
			return;
		}
		
		TStringArray split_file = {};
		FileName.Split(".", split_file);
		if (split_file.Count() != 0) {
			FileName += ".c";
		}

		string file = absolute_path + "/" + FileName;
		string command = string.Format("cmd /c copy NUL \"%1\"", file);
		
		
		PrintFormat("Creating new file %1", file);
		
		Workbench.RunCmd(command);
		
		FileName = string.Empty;
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel()
	{
		FileName = string.Empty;
	}
}