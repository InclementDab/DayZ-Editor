class EditorCameraMapMarker: EditorMarker
{
	protected Camera m_Camera;
	protected MapWidget m_EditorMap;
	
	void EditorCameraMapMarker(Camera camera)
	{	
		m_Camera = camera;
		m_EditorMap = GetEditor().GetEditorHud().EditorMapWidget;
	}
	
	override void Update()
	{
		if (!m_EditorMap || !m_EditorMap.IsVisible()) {
			Show(false);
			return;
		}
		
		Show(true);
				
		vector position = m_EditorMap.MapToScreen(m_Camera.GetPosition());
		SetPos(position[0], position[1]);
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/EditorCameraMarker.layout";
	}
}