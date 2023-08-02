[WorkbenchPluginAttribute("Configure Project", "Configure DayZ Project", "Ctrl+F6", "", {"ScriptEditor"})]
class PluginConfigureProject: PluginProject
{
	override void Run()
	{
		Workbench.ScriptDialog("Edit Environment", DIALOG_TAB_SIZE, m_LaunchSettings);
	}
	
	override void Configure()
	{
		Workbench.ScriptDialog("Edit Environment", DIALOG_TAB_SIZE, m_LaunchSettings);
	}
}