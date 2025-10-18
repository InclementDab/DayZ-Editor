class PluginErrorMessage
{
	static int Show(string message)
	{
		PluginErrorMessage msg();
		return Workbench.ScriptDialog("Error", message, msg);
	}
	
	[ButtonAttribute("Ok")]
	void Ok()
	{

	}
}
