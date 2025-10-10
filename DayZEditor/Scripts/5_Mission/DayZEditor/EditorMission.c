// Mission creation framework when?
#ifndef DF_MISSION_FRAMEWORK
Mission CreateEditorMission(string path)
{
	Print("Creating Mission: "+ path);
		
	if (path.Contains("MainMenu"))
	{
		//EditorMainMenuMission mm = new EditorMainMenuMission(path);
		//return mm;
	}
	
#ifdef DabsLabs
	return CreateDabsLabs(path);
#endif

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

#else

// MISSION FRAMEWORK NOW!!!
[RegisterMissionType(EditorMainMenuMissionWrapper, 105)]
class EditorMainMenuMissionWrapper: MissionWrapper
{
	override bool MissionLaunchCondition(string path)
	{
		return (path.Contains("MainMenu"));
	}
	
	override Mission CreateMission(string path)
	{
		return new EditorMainMenuMission(path);
	}
}

#endif

class EditorMainMenuMission: MissionBase
{
	protected ref EditorMainMenu m_MainMenu;
	protected ref IntroSceneEditor m_IntroSceneEditor;
	protected string m_Path;
	void EditorMainMenuMission(string path)
	{
		m_Path = path;
		
		if (CanLaunchCLE()) {
	        Hive ce = CreateHive();
	        if (ce) {
	            ce.InitOffline();
	        }
	    }
	}
	
	void ~EditorMainMenuMission()
	{
		if (GetHive()) {
			DestroyHive();	
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

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		
		if (m_IntroSceneEditor) {
	    	m_IntroSceneEditor.OnUpdate(timeslice);
		}
	}
	
	override void OnInit()
	{
		super.OnInit();
		
		// For reset cleanup
		GetGame().GetUIManager().CloseAll();

		string world_name;
		GetGame().GetWorldName(world_name);
		m_IntroSceneEditor = new IntroSceneEditor(world_name);
		
		m_MainMenu = new EditorMainMenu();
		m_IntroSceneEditor.MainMenuView = m_MainMenu;
	}
		
	override void Reset()
	{
		super.Reset();
		
		OnInit();
	}
	
	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		
		if (GetHive()) {
			DestroyHive();
		}
	}
}