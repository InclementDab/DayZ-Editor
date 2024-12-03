class EditorDeletedObjectWorldMarker: EditorMarker
{
	protected int m_ScreenX, m_ScreenY;
	protected MapWidget m_MapWidget;
	protected EditorDeletedObject m_EditorDeletedObject;
	
	void EditorDeletedObjectWorldMarker(EditorDeletedObject deleted_object)
	{
		EditorLog.Trace("EditorDeletedObjectWorldMarker");
		m_MapWidget = GetEditor().GetEditorHud().Map;
		m_EditorDeletedObject = deleted_object;
		GetScreenSize(m_ScreenX, m_ScreenY);
	}
	
	override void Update(float dt)
	{
		if (!m_LayoutRoot) {
			return;
		}
		
		if (!m_EditorDeletedObject.IsSelected()) {
			m_LayoutRoot.Show(false);
			return;
		}
		
		if (m_MapWidget && m_MapWidget.IsVisible()) {
			return;
		}
					
		// Is the marker in bounds?
		vector screen_pos = GetGame().GetScreenPos(m_EditorDeletedObject.GetBottomPosition());
		if (screen_pos[0] > 0 && screen_pos[0] <= m_ScreenX && screen_pos[1] > 0 && screen_pos[1] < m_ScreenY && screen_pos[2] > 0) {
			if (screen_pos[2] > m_ViewDistance) {
				m_LayoutRoot.Show(false);
				return;
			}
			
			Show(true);
			
			SetPos(screen_pos[0], screen_pos[1]);
			float c = m_ViewDistance / screen_pos[2];
			
			if (c < 6) {
				c = 6;
			}
			
			if (c > 28) {
				// Perplexity came up with this and it looks kinda awesome but it wont work for this application
				/*float t = (c - 32) / (128 - 32);  // Normalize to [0, 1] range
			    t = t * t * (3 - 2 * t);  // Smoothstep function
			    c = 32 + t * (128 - 32);*/   // Interpolate between 32 and 64

				c = 28 + (c - 28) / 4;
			}
				
			m_LayoutRoot.SetScreenSize(c, c);
		} else {
			Show(false);
		}
	}
}