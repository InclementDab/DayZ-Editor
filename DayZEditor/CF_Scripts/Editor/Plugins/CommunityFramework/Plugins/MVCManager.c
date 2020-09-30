[WorkbenchPluginAttribute("MVC Settings", "Edit MVC Settings", "", "", {"ResourceManager", "ScriptEditor"})]
class MVCManager: WorkbenchPlugin
{
	[Attribute("", "combobox", "", "", ParamEnumArray.FromEnum(LogLevel))]
	int Log_Level;
	
	override void Run()
	{
		Log_Level = MVCLogLevel;
		Workbench.ScriptDialog("Edit MVC Settings", "", this);
	}
	
	[ButtonAttribute("OK")]
	void DialogOk()
	{
		MVCLogLevel = Log_Level;
	}
		
	[ButtonAttribute("Cancel")]
	void DialogCancel()
	{
		
	}
}
 