[WorkbenchPluginAttribute("Join Remote", "Lanches DayZ and joins remote", "F5", "", {"ScriptEditor", "ResourceBrowser"})]
class PluginJoinRemote: PluginLaunchGameBase
{
	[Attribute("", "editbox")]
	string Address = "127.0.0.1";
	
	[Attribute("", "editbox")]
	int Port = 2302;
	
	[Attribute("", "editbox")]
	string Password;
	
	void PluginJoinRemote()
	{
		Workbench.ScriptDialog("Join Remote", DIALOG_TAB_SIZE, this);
	}
	
	[ButtonAttribute("Join", true)]
	void Join()
	{
		m_LaunchSettings.LaunchType = GameLaunchType.CLIENT;
		LaunchGame(m_LaunchSettings);
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel()
	{
	}
	
	override bool GetConnectionArguments(out string ip, out int port, out string password)
	{		
		ip = Address;
		port = Port;
		password = Password;
		return true;
	}
}