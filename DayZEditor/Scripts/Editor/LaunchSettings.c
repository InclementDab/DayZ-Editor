class LaunchSettings: SerializableBase
{
	static const int VERSION = 1;
	
	static const string CLIENT_PROFILE_NAME = "client";
	static const string SERVER_PROFILE_NAME = "server";
	static const string BASE_LAUNCH_PARAMS = "-newErrorsAreWarnings=1 -doLogs -adminlog -scriptDebug=true -profile -resizeable -window";
	
	protected string m_CurrentFileLocation;
	
	[Attribute("", "editbox", "Repository Directory")]
	string RepositoryDir;
	
	[Attribute("", "editbox", "Profile Directory")]
	string ProfileDir;		
	
	[Attribute("", "editbox", "Mission Directory")]
	string MissionDir;		
	
	[Attribute("", "editbox", "Mod Directory")]
	string ModDir;	
	
	[Attribute("", "combobox", "Launch Client", "", ParamEnumArray.FromEnum(EBool) )]
	bool Client;	
	
	[Attribute("", "combobox", "Launch Server", "", ParamEnumArray.FromEnum(EBool) )]
	bool Server;
	
	[Attribute("", "combobox", "Automatically attempt to connect to the local server", "", ParamEnumArray.FromEnum(EBool) )]
	bool AutoConnect;
		
	[Attribute("", "editbox", "Server password")]
	string ServerPassword;	
	
	[Attribute("", "editbox", "Map")]
	string Map;
		
	[Attribute("", "combobox", "Enable File Patching", "", ParamEnumArray.FromEnum(EBool) )]
	bool FilePatching;	
	
	[Attribute("", "combobox", "Enable Deloginator", "", ParamEnumArray.FromEnum(EBool) )]
	bool Deloginator;
	
	[Attribute("", "combobox", "Auto Close Game", "", ParamEnumArray.FromEnum(EBool) )]
	bool AutoClose;
			
	void Save(string file)
	{
		FileSerializer serializer = new FileSerializer();
		if (!serializer.Open(file, FileMode.WRITE)) {
			return;
		}
		
		Write(serializer, VERSION);
		serializer.Close();
	}
	
	static LaunchSettings Load(string file)
	{
		LaunchSettings settings = new LaunchSettings();
		settings.m_CurrentFileLocation = file;
		if (!FileExist(file)) {
			settings.AutoConnect = true;
			settings.Client = true;
			settings.Server = true;
			settings.ProfileDir = "P:\\profiles";
			settings.MissionDir = "P:\\missions";
			settings.ModDir = "P:\\mods";
			settings.Map = "ChernarusPlus";
			settings.AutoClose = true;
			settings.Save(file);
			return settings;
		}
		
		FileSerializer serializer = new FileSerializer();
		if (!serializer.Open(file, FileMode.READ)) {
			return null;
		}
		
		int version;
		if (!settings.Read(serializer, version)) {
			return null;
		}
		
		serializer.Close();
		return settings;
	}
	
	override void Write(Serializer serializer, int version)
	{
		serializer.Write(VERSION);
		serializer.Write(RepositoryDir);
		serializer.Write(ProfileDir);
		serializer.Write(MissionDir);
		serializer.Write(ModDir);
		serializer.Write(Client);
		serializer.Write(Server);
		serializer.Write(AutoConnect);
		serializer.Write(ServerPassword);
		serializer.Write(Map);
		serializer.Write(FilePatching);
		serializer.Write(AutoClose);
	}
	
	override bool Read(Serializer serializer, int version)
	{
		if (!serializer.Read(version)) {
			return false;
		}
		
		if (!serializer.Read(RepositoryDir)) {
			return false;
		}
		
		if (!serializer.Read(ProfileDir)) {
			return false;
		}		
		
		if (!serializer.Read(MissionDir)) {
			return false;
		}		
		
		if (!serializer.Read(ModDir)) {
			return false;
		}
			
		if (!serializer.Read(Client)) {
			return false;
		}			
		
		if (!serializer.Read(Server)) {
			return false;
		}		
				
		if (!serializer.Read(AutoConnect)) {
			return false;
		}		
											
		if (!serializer.Read(ServerPassword)) {
			return false;
		}		
				
		if (!serializer.Read(Map)) {
			return false;
		}		
		
		if (!serializer.Read(FilePatching)) {
			return false;
		}		
		
		if (!serializer.Read(AutoClose)) {
			return false;
		}
		
		return true;
	}
	
	[ButtonAttribute("Ok", true)]
	void Ok()
	{
		Save(m_CurrentFileLocation);
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel()
	{
	}
}