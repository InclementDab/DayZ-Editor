class EditorObjectWorldMarker: EditorObjectMarker
{
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
		m_LayoutRoot.Show(!state);
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
		vector screen_pos = GetGame().GetScreenPos(GetPosition());
		if (screen_pos[0] != 0 && screen_pos[0] != m_ScreenX && screen_pos[1] != 0 && screen_pos[1] != m_ScreenY && screen_pos[2] > 0) {
			SetPos(screen_pos[0], screen_pos[1]);
			Show(m_Show);
		} 
		
		// Overrides the hide if the camera isnt looking at the marker
		else { 
			m_LayoutRoot.Show(false);
		}
	}
	
	protected vector GetPosition()
	{		
		// Should the position be raycasted on the ground, or locked to the object
		if (m_Editor.GroundMode) {
			vector position;
			vector object_transform[4];
			m_EditorObject.GetTransform(object_transform);
			set<Object> o;
			vector ground_dir; int component;
			DayZPhysics.RaycastRV(object_transform[3], object_transform[3] + object_transform[1] * -1000, position, ground_dir, component, o, NULL, m_EditorObject.GetWorldObject(), false, true); // set to ground only
			return position;
		} 
		
		return m_EditorObject.GetBottomCenter();
	}
	
	override void Show(bool show)
	{
		if (vector.Distance(m_Editor.GetCamera().GetPosition(), GetPosition()) > m_Editor.Settings.MarkerViewDistance) {
			m_LayoutRoot.Show(false);
			return;
		}
		
		super.Show(show);
	}
}