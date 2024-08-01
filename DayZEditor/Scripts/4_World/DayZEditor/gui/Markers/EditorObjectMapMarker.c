class EditorObjectMapMarker: EditorObjectMarker
{		
	private MapWidget m_EditorMap;
	
	void EditorObjectMapMarker(EditorObject editor_object)
	{
		m_DragHandler = new EditorMapDragHandler();
		m_EditorMap = m_Editor.GetEditorHud().EditorMapWidget;
		
		GetLayoutRoot().SetSort(101);
	}
	
	override void Update(float dt)
	{
		if (!m_EditorMap || !m_EditorMap.IsVisible()) { 
			return;
		}
		
		vector position = m_EditorMap.MapToScreen(m_EditorObject.GetPosition());
		SetPos(position[0], position[1]);
		super.Update(dt);
	}
}