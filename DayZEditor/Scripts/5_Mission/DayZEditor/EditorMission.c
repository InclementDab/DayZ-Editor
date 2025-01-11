class EditorMainMenuMission: MissionBase
{
	protected ref EditorMainMenu m_MainMenu;
	protected ref IntroSceneEditor m_IntroSceneEditor;
	protected string m_Path;
	void EditorMainMenuMission(string path)
	{
		m_Path = path;
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