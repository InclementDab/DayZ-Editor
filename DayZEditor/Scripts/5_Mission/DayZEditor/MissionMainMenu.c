modded class MissionMainMenu
{
	protected ref EditorMainMenu m_MainMenu;
	protected ref IntroSceneEditor m_IntroSceneEditor;

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		
		if (m_IntroSceneEditor) {
	    	m_IntroSceneEditor.OnUpdate(timeslice);
		}
	}
	
	override void OnInit()
	{
		// For reset cleanup
		GetGame().GetUIManager().CloseAll();

		if (!m_NoCutscene) {
			string world_name;
			GetGame().GetWorldName(world_name);
			m_IntroSceneEditor = new IntroSceneEditor(world_name);
		}
		
		m_MainMenu = new EditorMainMenu();
		m_IntroSceneEditor.MainMenuView = m_MainMenu;
		m_mainmenu = m_MainMenu.GetUIScriptViewMenu();
		
		GetOnInputDeviceChanged().Insert(OnInputDeviceChanged);
	}
}