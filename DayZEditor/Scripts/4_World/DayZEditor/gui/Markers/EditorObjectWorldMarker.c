class EditorObjectWorldMarker: EditorObjectMarker
{
	protected MapWidget m_MapWidget;
	void EditorObjectWorldMarker(EditorObject editor_object)
	{
		m_MapWidget = m_Editor.GetEditorHud().EditorMapWidget;
		m_DragHandler = new ObjectDragHandler(m_EditorObject);
		EditorEvents.OnMapToggled.Insert(OnEditorMapToggled);
	}
	
	void OnEditorMapToggled(Class context, MapWidget editor_map, bool state)
	{
		m_LayoutRoot.Show(!state);
	}
	
	override void Update()
	{
		if (m_MapWidget && m_MapWidget.IsVisible()) {
			return;
		}
				
		vector screen_pos = GetGame().GetScreenPos(GetPosition());
						
		int screen_x, screen_y;
		GetScreenSize(screen_x, screen_y);
		// Is the marker in bounds?
		if (screen_pos[0] != 0 && screen_pos[0] != screen_x && screen_pos[1] != 0 && screen_pos[1] != screen_y && screen_pos[2] > 0) {
			SetPos(screen_pos[0], screen_pos[1]);
			Show(m_Show);
		} 
		
		// Overrides the hide if the camera isnt looking at the marker
		else if (m_LayoutRoot)
			m_LayoutRoot.Show(false);		

		
		super.Update();
	}
	
	protected vector GetPosition()
	{
		vector position;
		vector object_transform[4];
		m_EditorObject.GetTransform(object_transform);
		
		// Should the position be raycasted on the ground, or locked to the object
		if (m_Editor.GroundMode) {
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
	
	/*
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorObjectWorldMarker::OnClick: " + button);
		
		switch (button) {
			
			case MouseState.RIGHT: {
				GetEditor().SelectObject(m_EditorObject);
				EditorContextMenu context_menu = new EditorContextMenu();
				context_menu.SetPosition(x, y);
				
				return true;
			}			
		}
		
		return false;
	}
	
	/*
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorObjectWorldMarker::OnClick: " + button);
		
		if (button == 0) {
			
			if (GetEditor().IsPlacing()) return false;
			
			// We want to Toggle selection if you are holding control
			if (KeyState(KeyCode.KC_LCONTROL))
				GetEditor().ToggleSelection(m_EditorObject);
			else {
				// required for multiple objects to be dragged
				if (m_EditorObject.IsSelected()) 
					return true;
				
				if (!KeyState(KeyCode.KC_LSHIFT))
					GetEditor().ClearSelection();
				
				GetEditor().SelectObject(m_EditorObject);
			}	
		}
		
		return super.OnClick(w, x, y, button);
	}

	*/
}