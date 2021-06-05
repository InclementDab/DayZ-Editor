class EditorDeletedObjectWorldMarker: EditorMarker
{
	protected int m_ScreenX, m_ScreenY;
	protected MapWidget m_MapWidget;
	protected EditorDeletedObject m_EditorDeletedObject;
	
	void EditorDeletedObjectWorldMarker(EditorDeletedObject deleted_object)
	{
		EditorLog.Trace("EditorDeletedObjectWorldMarker");
		m_MapWidget = GetEditor().GetEditorHud().EditorMapWidget;
		m_EditorDeletedObject = deleted_object;
		GetScreenSize(m_ScreenX, m_ScreenY);
		SetColor(LIST_ITEM_COLOR_ON_DELETED);
	}
	
	override void Update()
	{
		if (!m_LayoutRoot) {
			return;
		}
		
		if (m_MapWidget && m_MapWidget.IsVisible()) {
			return;
		}
			
		// Is the marker in bounds?
		vector screen_pos = GetGame().GetScreenPos(m_EditorDeletedObject.GetBottomPosition());
		if (screen_pos[0] != 0 && screen_pos[0] != m_ScreenX && screen_pos[1] != 0 && screen_pos[1] != m_ScreenY && screen_pos[2] > 0) {
			SetPos(screen_pos[0], screen_pos[1]);
			Show(true);
		} 
		
		// Overrides the hide if the camera isnt looking at the marker
		else { 
			m_LayoutRoot.Show(false);
		}
	}
}