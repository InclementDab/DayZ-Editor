class EditorObjectWorldMarker: EditorObjectMarker
{
	static const float MARKER_MIN_SIZE = 10;
	static const float MARKER_MAX_SIZE = 22;
	
	protected int m_ScreenX, m_ScreenY;
	protected MapWidget m_MapWidget;
	void EditorObjectWorldMarker(EditorObject editor_object)
	{
		m_MapWidget = m_Editor.GetEditorHud().EditorMapWidget;
		m_DragHandler = new EditorObjectDragHandler(m_EditorObject);
		EditorEvents.OnMapToggled.Insert(OnEditorMapToggled);
		
		GetScreenSize(m_ScreenX, m_ScreenY);
	}
	
	void OnEditorMapToggled(Class context, MapWidget editor_map, bool state)
	{
		m_Show = !state;
	}
	
	override void Update()
	{		
		vector position = GetPosition();	
		float distance = vector.Distance(GetGame().GetCurrentCameraPosition(), position);
		if (m_Editor.Settings.MarkerViewDistance < distance) {
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
		
		
		float size = Math.Min(Math.Max(1000 / distance, MARKER_MIN_SIZE), MARKER_MAX_SIZE);
		SetSize(size, size);
		SetPos(screen_pos[0], screen_pos[1]);
		Show(m_Show);
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
}