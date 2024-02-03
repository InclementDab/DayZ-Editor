modded class MissionMainMenu
{
	protected ref EditorMainMenu m_EditorMainMenu;
		
	override void OnInit()
	{
		GetOnInputDeviceChanged().Insert(OnInputDeviceChanged);
	}
	
	override void CreateIntroScene()
	{
	}
	
	override void Reset()
	{		
		delete m_EditorMainMenu;
		m_EditorMainMenu = new EditorMainMenu();
	}
	
	override void OnUpdate(float timeslice)
	{
	}
	
	override void OnMissionStart()
	{
		if (!m_EditorMainMenu) {
			m_EditorMainMenu = new EditorMainMenu();
		}
				
		// Set game state
		GetDayZGame().SetMissionState(DayZGame.MISSION_STATE_MAINMENU);
		GetDayZGame().SetGameState(DayZGameState.MAIN_MENU);
		GetDayZGame().SetLoadState(DayZLoadState.MAIN_MENU_START);	
				
		ProgressAsync.DestroyAllPendingProgresses();
		
		// i was just annoyed
#ifndef DIAG_DEVELOPER
		PlayMusic();
#endif
	}
	
	override void OnMissionFinish()
	{
		delete m_EditorMainMenu;
	}
		
	EditorMainMenu GetMainMenu()
	{
		return m_EditorMainMenu;
	}
}