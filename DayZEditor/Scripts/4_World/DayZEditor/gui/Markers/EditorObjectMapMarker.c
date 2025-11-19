class EditorObjectMapMarker: EditorObjectMarker
{		
	private MapWidget m_EditorMap;
	
	void EditorObjectMapMarker(EditorObject editor_object)
	{
		m_EditorMap = m_Editor.GetEditorHud().Map;
		
		GetLayoutRoot().SetSort(101);
	}
	
	override void Update(float dt)
	{
		// Strict null check on the widget object
        if (!m_EditorMap) 
        {
             // Try to re-acquire
            if (m_Editor && m_Editor.GetEditorHud())
                m_EditorMap = m_Editor.GetEditorHud().Map;
        }

        // If still null, or if the widget is dead (invisible/destroyed), abort.
        if (!m_EditorMap) { 
            m_LayoutRoot.Show(false);
            return;
        }

		if (!m_EditorMap || !m_EditorMap.IsVisible()) { 
			m_LayoutRoot.Show(false);
			return;
		}
		
		m_LayoutRoot.Show(true);
		vector position = m_EditorMap.MapToScreen(m_EditorObject.GetPosition());
		SetPos(position[0], position[1]);
		SetSize(0.5);
		super.Update(dt);
	}

	override typename GetDragHandlerType()
	{
		return EditorMapDragHandler;
	}
}