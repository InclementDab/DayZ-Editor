class EditorObjectWorldMarker: EditorObjectMarker
{	
	protected int m_ScreenX, m_ScreenY;
	protected MapWidget m_MapWidget;
	void EditorObjectWorldMarker(EditorObject editor_object)
	{
		m_MapWidget = m_Editor.GetEditorHud().EditorMapWidget;
		EditorEvents.OnMapToggled.Insert(OnEditorMapToggled);
		
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
		float marker_view_distance_sq = m_Editor.GetSettings().MarkerViewDistance * m_Editor.GetSettings().MarkerViewDistance;
		if (marker_view_distance_sq < distancesq) {
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
		float size_01 = Math.InverseLerp(0, marker_view_distance_sq, distancesq);
		
		SetSize(size_01);
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