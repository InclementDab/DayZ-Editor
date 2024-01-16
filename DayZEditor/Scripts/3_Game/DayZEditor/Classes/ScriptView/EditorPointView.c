class EditorPointView: ScriptView
{	
	Object Preview;
	vector Offset;
	
	ImageWidget Image;
	
	override void Update(float dt)
	{
		super.Update(dt);
		
		if (!Preview) {
			return;
		}
		
		vector transform[4];
		Preview.GetTransform(transform);
			
		vector screen_position = GetGame().GetScreenPos(Offset.Multiply4(transform));
		if (m_LayoutRoot) {
			m_LayoutRoot.SetPos(screen_position[0], screen_position[1]);
			m_LayoutRoot.Show(screen_position[2] > 0);
		}
	}
	
	bool DoCursorRaycast(out vector position, float max_distance = 3000, Object ignore_object = null)
	{
		vector raycast_direction = GetGame().GetPointerDirection();		
		vector begin_pos = GetGame().GetCurrentCameraPosition();
		vector end_pos = begin_pos + raycast_direction * max_distance;
		int interaction_layers = PhxInteractionLayers.BUILDING | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.DYNAMICITEM | PhxInteractionLayers.ITEM_LARGE;
		Object hit_object;
		vector normal;
		float fraction;
		return DayZPhysics.RayCastBullet(begin_pos, end_pos, interaction_layers, ignore_object, hit_object, position, normal, fraction);
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		vector position;
		DoCursorRaycast(position, 3000, Preview);
		
		Preview.SetPosition(position);
		
		return super.OnDragging(w, x, y, reciever);
	}
	
	/*override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{		
		// ignores the object if you are placing
		if (m_Editor.IsPlacing()) { 
			return false;
		}
		
		// Delete the current tooltip to clean the UI a bit
		//GetEditor().GetEditorHud().SetCurrentTooltip(null);
		
		switch (button) {
			
			case MouseState.LEFT: {
				
				// We want to Toggle selection if you are holding control
				if (KeyState(KeyCode.KC_LCONTROL)) {
					m_EditorObject.SetSelected(!m_EditorObject.IsSelected());
					return true;
				} 
				
				// allows multiple objects to be dragged
				if (m_EditorObject.IsSelected()) {
					thread CheckDragBounds(x, y);
					return true;
				}
				
				if (!KeyState(KeyCode.KC_LSHIFT)) {
					EditorObject.ClearSelections();
				}
				
				m_EditorObject.SetSelected(true);
				
				thread CheckDragBounds(x, y);
				return false;
			}
			
			case MouseState.MIDDLE: {
				EditorCamera camera = GetEditor().GetCamera();
				vector pos = m_EditorObject.GetWorldObject().GetPosition();
				pos[1] = camera.GetPosition()[1];
				camera.SendToPosition(pos);
				return true;
			}
			
			case MouseState.RIGHT: {
				
				if (!m_EditorObject.IsSelected() && !KeyState(KeyCode.KC_LSHIFT)) {
					EditorObject.ClearSelections();
				}
				
				m_EditorObject.SetSelected(true);
				
				if (EditorHud.CurrentMenu) {
					delete EditorHud.CurrentMenu;
				}
				
				EditorHud.CurrentMenu = new EditorPlacedContextMenu(x, y, m_EditorObject);
				
				return true;
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}*/
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		WidgetAnimator.AnimateColor(Image, ARGB(255, 44, 147, 55), 100);
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		WidgetAnimator.AnimateColor(Image, ARGB(255, 255, 255, 255), 100);
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\EditorPoint.layout";
	}
}