// Mission creation framework when?
Mission CreateEditorMission(string path)
{
	Print("Creating Mission: "+ path);
	
	// g_Game.SetMissionPath(path); Done from C++ now

	if (g_Game.IsMultiplayer() && g_Game.IsServer())
	{
		return new MissionServer;
	}

#ifdef NO_GUI
	return new MissionDummy;
#endif
	MissionMainMenu m;
	if (path.Contains("NoCutscene"))
	{
		m = new MissionMainMenu();
		m.m_NoCutscene = true;
		return m;
	}
	
	if (path.Contains("MainMenu"))
	{
		m = new MissionMainMenuEditorDummy(path);
		m.m_NoCutscene = false;
		return m;
	}
	else
	{
		if( path == "" )
		{
			return new MissionDummy;
		}
#ifndef NO_GUI_INGAME
		return new MissionGameplay;
#else
		return new MissionDummy;
#endif
	}
}

class MissionMainMenuEditorDummy: MissionMainMenu
{
    protected string m_Path;
    void MissionMainMenuEditorDummy(string path)
    {
        m_Path = path;
		
	    if (CanLaunchCLE()) {
	        Hive ce = CreateHive();
	        if (ce) {
	            ce.InitOffline();
	        }
	    }
    }
	
	void ~MissionMainMenuEditorDummy()
	{
		if (GetHive()) {
			DestroyHive();	
		}
		
		string directory = Directory.GetDirectory(m_Path);
		if (FileExist(SystemPath.Combine(directory, "storage_-1"))) {
			DeleteFile(SystemPath.Combine(directory, "storage_-1"));
		}
	}
		
    bool CanLaunchCLE()
    {
        for (int i = 0; i < m_Path.LengthUtf8(); i++) {
            string char = m_Path.SubstringUtf8(i, 1);
            if (char.ToAscii() < 0) {
                return false;
            }
        }

        return true;
    }
}