class PluginProject: PluginDialogBase
{
	static const string PROJECT_CFG = "project.cfg";
	static const string PREFIX_CFG = "prefixes.cfg";
	static const string LAUNCH_BIN = "launch.bin";
	static const string BUILD_BIN = "build.bin";

//#ifdef DIAG_DEVELOPER
	static const string EXECUTABLE = "DayZDiag_x64.exe";
//#else
//	static const string EXECUTABLE = "DayZ_x64.exe";
//#endif
	
	protected ref map<string, string> m_ProjectSettings = new map<string, string>();
	protected ref array<string> m_Prefixes = {};
	
	protected ref LaunchSettings m_LaunchSettings;
	protected ref BuildSettings m_BuildSettings;
	
	void PluginProject()
	{
		string current_dir = GetCurrentDirectory();
		if (!LoadConfig(current_dir + PROJECT_CFG, m_ProjectSettings)) {
			ErrorDialog(string.Format("Failed to load %1 in %2", PROJECT_CFG, current_dir));
			return;
		}

		// Load launch settings
		m_LaunchSettings = LaunchSettings.Load(current_dir + LAUNCH_BIN);
		if (!m_LaunchSettings) {
			ErrorDialog(string.Format("Failed to load %1 in %2", LAUNCH_BIN, current_dir));
			return;
		}
		
		// Load build settings
		m_BuildSettings = BuildSettings.Load(current_dir + BUILD_BIN);
		if (!m_BuildSettings) {
			ErrorDialog(string.Format("Failed to load %1 in %2", BUILD_BIN, current_dir));
			return;
		}
		
		LoadPrefixes(current_dir + PREFIX_CFG, m_Prefixes);
	}
			
	protected bool LoadConfig(string file, inout map<string, string> settings)
	{
		if (!FileExist(file)) {
			return false;
		}

		FileHandle handle = OpenFile(file, FileMode.READ);
		if (!handle) {
			return false;
		}
		
		string data, buffer;
		while (ReadFile(handle, buffer, 256) != 0) {
		    data += buffer;
		}
		
		CloseFile(handle);
		
		array<string> lines = {};
		data.Split("\n", lines);
		
		foreach (string line: lines) {
			line.TrimInPlace();
			line.Replace("\t", "");
			line.Replace("\n", "");
			line.Replace("\r", "");
			if (line == string.Empty) {
				continue;
			}
			
			string key = string.Empty;
			string token = string.Empty;
			
			// search forward for first equals sign			
			while (line.ParseStringEx(token) > 1) {
				key += token;
			}
			
			settings[key] = line;
		}
		
		return true;
	}
	
	protected bool LoadPrefixes(string file, inout array<string> lines)
	{
	    FileHandle handle = OpenFile(file, FileMode.READ);
		if (!handle) {
			return false;
		}
	
	    string data, buffer;
	    while (ReadFile(handle, buffer, 256) != 0) {
	        data += buffer;
		}
	
	    data.Split("\n", lines);
	    CloseFile(handle);
		return true;
	}
}