[WorkbenchPluginAttribute("Launch Game", "Lanches DayZ", "F6", "", {"ScriptEditor", "ResourceBrowser"})]
class PluginLaunchGame: PluginLaunchGameBase
{
	void PluginLaunchGame()
	{
		KillTask(EXECUTABLE);
		LaunchGame(m_LaunchSettings);
	}
	
	override bool GetConnectionArguments(out string ip, out int port, out string password)
	{
		if (m_LaunchSettings.LaunchType != GameLaunchType.BOTH) {
			return false;
		}
		
		ip = "127.0.0.1";
		port = 2302;
		password = m_LaunchSettings.ServerPassword;
		return true;
	}
}