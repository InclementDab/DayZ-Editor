class EditorMainMenuController: ViewController
{	
	protected Camera m_Camera;
	protected ref EditorMainMenuScene m_Scene;
	
	void EditorMainMenuController()
	{
		m_Scene = EditorMainMenuScene.GetRandomEntry(GetGame().GetWorldName());
		
		m_Camera = Camera.Cast(GetGame().CreateObject("Camera", m_Scene.CameraPosition));
		m_Camera.SetFOV(m_Scene.CameraFov);
		m_Camera.SetActive(true);
		m_Camera.SetDirection(m_Scene.CameraDirection);
		m_Camera.Update();
		
		//GetGame().CreateObject("ClutterCutter2x2", "1130.31 5.9941 2396.33");
	}
}