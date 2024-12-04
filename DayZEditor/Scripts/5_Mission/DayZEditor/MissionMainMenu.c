modded class MissionMainMenu
{
	protected ref EditorMainMenu m_MainMenu;

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		
		if (m_IntroScenePC)
	    	m_IntroScenePC.OnUpdate(timeslice);
	}
	
	override void OnInit()
	{
		if (!m_NoCutscene)
		{
			CreateIntroScene();
		}
		
		delete m_MainMenu;
		m_MainMenu = new EditorMainMenu();
		m_mainmenu = m_MainMenu.GetUIScriptViewMenu();
		
		GetOnInputDeviceChanged().Insert(OnInputDeviceChanged);
	}
	
	override void PlayMusic()
	{
	}
}