class PluginLaunchGameBase: PluginProject
{	
	void LaunchGame(notnull LaunchSettings launch_settings)
	{
		string root = GetRootDirectory();
		string mod_prefix = GetPrefix();
		string workbench_directory = GetWorkbenchDirectory();
						
		//! Game launch script
		// append prefix of current mod
		m_ProjectSettings["Mods"] = m_ProjectSettings["Mods"] + ";@" + mod_prefix;
		
		// finding DayZ / DayZ Exp dir		
		string game_directory = GetSourceDataDirectory();		
		string game_exe = game_directory + EXECUTABLE;
		if (!FileExist(game_exe)) {
			ErrorDialog(string.Format("Could not find the game at %1", game_exe));
			return;
		}
		
		if (launch_settings.Repository == string.Empty) {
			ErrorDialog("You need to set the Repository setting in Plugins -> Configure -> Configure Project");
			return;
		}		
		
		if (launch_settings.Profiles == string.Empty) {
			ErrorDialog("You need to set the Profiles setting in Plugins -> Configure -> Configure Project");
			return;
		}		
		
		if (launch_settings.Missions == string.Empty) {
			ErrorDialog("You need to set the Missions setting in Plugins -> Configure -> Configure Project");
			return;
		}		
		
		if (launch_settings.Mods == string.Empty) {
			ErrorDialog("You need to set the Mods setting in Plugins -> Configure -> Configure Project");
			return;
		}		
		
		if (launch_settings.ServerConfig == string.Empty) {
			ErrorDialog("You need to set the ServerConfig setting in Plugins -> Configure -> Configure Project");
			return;
		}
		
		if (launch_settings.AutoClose) {
			KillTask(EXECUTABLE);
		}
		
		// Set up symlinks so game can launch with our cwd
		PromiseSymLink(game_directory + PATH_SEPERATOR_ALT + "Addons", workbench_directory + PATH_SEPERATOR_ALT + "Addons");
		PromiseSymLink(game_directory + PATH_SEPERATOR_ALT + "bliss", workbench_directory + PATH_SEPERATOR_ALT + "bliss");
		
		if (!FileExist(launch_settings.ServerConfig)) {
			CopyFile(launch_settings.Repository + "\\ServerDZ.cfg", launch_settings.ServerConfig);
		}
		
		// Set up filepatching, needs to either create or delete all links depending on the setting
		if (launch_settings.FilePatching) {
			foreach (string prefix: m_Prefixes) {
				prefix.TrimInPlace();
				prefix.Replace("\t", "");
				prefix.Replace("\n", "");
				prefix.Replace("\r", "");
				MakeDirectory(workbench_directory + PATH_SEPERATOR_ALT + prefix + "\\..\\");
				PromiseSymLink(root + PATH_SEPERATOR_ALT + prefix, workbench_directory + PATH_SEPERATOR + prefix);
			}
		} else {
			// Delete all extra folders in wb directory			
			string wb_dir_filename;
			FileAttr wb_dir_fileattr;
			FindFileHandle hdnl = FindFile(workbench_directory + PATH_SEPERATOR + "*", wb_dir_filename, wb_dir_fileattr, FindFileFlags.DIRECTORIES);
			
			if (WB_DIR_DEFAULTS.Find(wb_dir_filename) == -1) {
				DeleteFile(wb_dir_filename);
			}
			
			while (FindNextFile(hdnl, wb_dir_filename, wb_dir_fileattr)) {
				if (WB_DIR_DEFAULTS.Find(wb_dir_filename) == -1 && wb_dir_fileattr == FileAttr.DIRECTORY) {
					Workbench.RunCmd(string.Format("cmd /c rmdir /s /q \"%1\"", workbench_directory + PATH_SEPERATOR + wb_dir_filename));
				}
			}
			
			CloseFindFile(hdnl);
		}
		
		// Reformats mod list
		string formatted_mod_list;
		array<string> mod_list = {};
		m_ProjectSettings["Mods"].Split(";", mod_list);
		for (int i = 0; i < mod_list.Count(); i++) {
			formatted_mod_list += launch_settings.Mods + PATH_SEPERATOR + mod_list[i];
			if (i != mod_list.Count() - 1) {
				formatted_mod_list += ";";
			}
		}
		
		// Reformats server mod list
		string formatted_server_mod_list;
		array<string> server_mod_list = {};
		m_ProjectSettings["ServerMods"].Split(";", server_mod_list);
		for (int j = 0; j < server_mod_list.Count(); j++) {
			formatted_server_mod_list += launch_settings.Mods + PATH_SEPERATOR + server_mod_list[j];
			if (j != server_mod_list.Count() - 1) {
				formatted_server_mod_list += ";";
			}
		}
		
		MakeDirectory(launch_settings.Profiles);
		MakeDirectory(launch_settings.Missions);
		
		string client_profile_directory = string.Format("%1\\%2", launch_settings.Profiles, LaunchSettings.CLIENT_PROFILE_NAME);
		string server_profile_directory = string.Format("%1\\%2", launch_settings.Profiles, LaunchSettings.SERVER_PROFILE_NAME);		
		string server_mission = string.Format("%1\\%2.%3", launch_settings.Missions, mod_prefix, launch_settings.Map);
		
		// Make the folders if they dont exist yet
		MakeDirectory(client_profile_directory);
		MakeDirectory(server_profile_directory);
		MakeDirectory(server_mission);
		
		// Always clean them log folders
		if (launch_settings.Deloginator) {
			CleanLogFolder(client_profile_directory);
			CleanLogFolder(server_profile_directory);
		}
		
		// Copy maps and mission info
		CopyFiles(string.Format("%1\\Profiles\\Maps\\%2", launch_settings.Repository, launch_settings.Map), server_profile_directory);
		CopyFiles(string.Format("%1\\Profiles\\Global", launch_settings.Repository), server_profile_directory);
		
		CopyFiles(string.Format("%1\\Missions\\%3.%2", launch_settings.Repository, launch_settings.Map, mod_prefix), server_mission);
		CopyFiles(string.Format("%1\\Missions\\Global", launch_settings.Repository), server_mission);
		CopyFiles(string.Format("%1\\Missions\\Dev", launch_settings.Repository), server_mission);
		
		string client_launch_params = LaunchSettings.BASE_LAUNCH_PARAMS + string.Format(" \"-mod=%1\" \"-profiles=%2\"", formatted_mod_list, client_profile_directory);
		string server_launch_params = LaunchSettings.BASE_LAUNCH_PARAMS + string.Format(" \"-mod=%1\" \"-profiles=%2\" \"-serverMod=%3\" \"-config=%4\" \"-mission=%5\" -server", formatted_mod_list, server_profile_directory, formatted_server_mod_list, launch_settings.ServerConfig, server_mission);
		string offline_launch_params = LaunchSettings.BASE_LAUNCH_PARAMS + string.Format(" \"-mod=%1\" \"-profiles=%2\" \"-mission=%3\"", formatted_mod_list, client_profile_directory, server_mission);
		
		string ip, password;
		int port;
		if (GetConnectionArguments(ip, port, password)) {
			client_launch_params += string.Format(" -connect=%1 -port=%2 -password=%3", ip, port, password);
			server_launch_params += string.Format(" -port=%1", port);
		}
		
		if (launch_settings.FilePatching) {
			client_launch_params += " -filePatching";
			server_launch_params += " -filePatching";
			offline_launch_params += " -filePatching";
		}
				
		if ((launch_settings.LaunchType & GameLaunchType.CLIENT) == GameLaunchType.CLIENT) {
			Workbench.RunCmd(string.Format("%1 %2", game_exe, client_launch_params));
		}	
		
		if ((launch_settings.LaunchType & GameLaunchType.SERVER) == GameLaunchType.SERVER) {
			Workbench.RunCmd(string.Format("%1 %2", game_exe, server_launch_params));
		}
		
		if ((launch_settings.LaunchType & GameLaunchType.OFFLINE) == GameLaunchType.OFFLINE) {
			Workbench.RunCmd(string.Format("%1 %2", game_exe, offline_launch_params));
		}		
	}
	
	bool GetConnectionArguments(out string ip, out int port, out string password)
	{
		return false;
	}
}