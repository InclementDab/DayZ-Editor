[WorkbenchPluginAttribute("Configure Build Game", "Configures the build tools", "Ctrl+F8", "", {"ScriptEditor", "ResourceBrowser"})]
class PluginConfigureBuild: PluginProject
{
	override void Run()
	{
		Workbench.ScriptDialog("Edit Build", DIALOG_TAB_SIZE, m_BuildSettings);
	}
	
	override void Configure()
	{
		Workbench.ScriptDialog("Edit Build", DIALOG_TAB_SIZE, m_BuildSettings);
	}
}