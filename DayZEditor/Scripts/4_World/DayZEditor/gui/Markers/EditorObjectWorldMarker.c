class EditorObjectWorldMarker: EditorObjectMarker
{	
	protected int m_ScreenX, m_ScreenY;
	protected MapWidget m_MapWidget;
	protected float m_MarkerDistSq;
	void EditorObjectWorldMarker(EditorObject editor_object)
	{
		m_MapWidget = m_Editor.GetEditorHud().Map;
		EditorEvents.OnMapToggled.Insert(OnEditorMapToggled);
		m_MarkerDistSq = m_Editor.GetSettings().MarkerViewDistance * m_Editor.GetSettings().MarkerViewDistance;
		GetScreenSize(m_ScreenX, m_ScreenY);
	}
	
	void OnEditorMapToggled(Class context, MapWidget editor_map, bool state)
	{
		m_Show = !state;
	}
	
	override void Update(float dt)
	{
		vector position = GetPosition();	
		float distancesq = vector.DistanceSq(GetGame().GetCurrentCameraPosition(), position);
		if (!m_Editor.GetEditorHud().IsVisible()) {
			m_LayoutRoot.Show(false);
			return;
		}
		
		if (m_MarkerDistSq < distancesq) {
			m_LayoutRoot.Show(false);
			return;
		}
			
		vector screen_pos = GetGame().GetScreenPos(position);
		// Overrides the hide if the camera isnt looking at the marker
		if (screen_pos[0] == 0 || screen_pos[0] == m_ScreenX || screen_pos[1] == 0 || screen_pos[1] == m_ScreenY || screen_pos[2] < 0) {
			m_LayoutRoot.Show(false);
			return;
		}
		
		if (m_EditorObject.IsAttachedToObject()) {
			m_LayoutRoot.Show(false);
			return;
		}
						
		//float size = Math.Min(Math.Max(1300 / distancesq, 18), 24);
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
		
		SetPos(screen_pos[0], screen_pos[1]);
		bool show = m_Show && !GetEditor().IsMapActive();
		Show(show);
	}
	
	protected vector GetPosition()
	{		
		// Should the position be raycasted on the ground, or locked to the object
		if (m_Editor.GroundMode) {
			vector position;
			vector object_transform[4];
			m_EditorObject.GetTransform(object_transform);
			vector ground_dir; int component;
			DayZPhysics.RaycastRV(object_transform[3], object_transform[3] + object_transform[1] * -1000, position, ground_dir, component, null, null, m_EditorObject.GetWorldObject(), false, true); // set to ground only
			return position;
		} 
		
		return m_EditorObject.GetBottomCenter();
	}
	
	
	override typename GetDragHandlerType()
	{
		return EditorObjectDragHandler;
	}
}