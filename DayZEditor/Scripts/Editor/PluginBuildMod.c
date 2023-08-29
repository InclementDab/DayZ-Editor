[WorkbenchPluginAttribute("Build Mod", "Build the whole damn mod... all of it!", "F8", "", {"ScriptEditor"})]
class PluginBuildMod: PluginProject
{
	void PluginBuildMod()
	{
		if (m_LaunchSettings.Repository == string.Empty) {
			ErrorDialog("You need to set the Repository setting in Plugins -> Configure -> Configure Project");
			return;
		}		
		
		if (m_LaunchSettings.Profiles == string.Empty) {
			ErrorDialog("You need to set the Profiles setting in Plugins -> Configure -> Configure Project");
			return;
		}		
		
		if (m_LaunchSettings.Missions == string.Empty) {
			ErrorDialog("You need to set the Missions setting in Plugins -> Configure -> Configure Project");
			return;
		}		
		
		if (m_LaunchSettings.Mods == string.Empty) {
			ErrorDialog("You need to set the Mods setting in Plugins -> Configure -> Configure Project");
			return;
		}
		
		string mod_prefix = GetPrefix();
		// Set up our sym link for workshop modding :drool:
		string game_directory_stable = GetSourceDataDirectory();
		if (!FileExist(string.Format("%1\\!Workshop", game_directory_stable))) {
			Print("Couldnt find workshop folder in current directory, probably experimental.");
			game_directory_stable = GetDirectory(game_directory_stable) + "\\DayZ";
			if (!FileExist(string.Format("%1\\!Workshop", game_directory_stable))) {
				ErrorDialog("Could not find DayZ Workshop folder. Is your Source Data correctly set?");
				return;
			}
		}
		
		PromiseSymLink(string.Format("%1\\!Workshop", game_directory_stable), m_LaunchSettings.Mods);				
		// Set up our mod output correctly if not done so already
		string mod_output = string.Format("%1\\@%2", m_LaunchSettings.Mods, mod_prefix);
		
		MakeDirectory(mod_output);
		MakeDirectory(mod_output + PATH_SEPERATOR_ALT + "Addons");
		MakeDirectory(mod_output + PATH_SEPERATOR_ALT + "Keys");
		
		CopyFiles(string.Format("%1\\Addons", m_LaunchSettings.Repository), mod_output + PATH_SEPERATOR_ALT + "Addons");
		
		Workbench.RunCmd(string.Format("pboProject -Mod=%1 P:\\%2", mod_output, mod_prefix));
	}
}