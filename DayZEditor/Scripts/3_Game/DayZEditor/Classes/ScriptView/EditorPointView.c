class EditorPointView: ScriptView
{	
	protected EditorObject m_EditorObject;
	protected vector m_Offset;
	
	ImageWidget Image;
	
	void EditorPointView(notnull EditorObject editor_object, vector offset)
	{
		m_EditorObject = editor_object;
		m_Offset = offset;
	}
	
	override void Update(float dt)
	{
		super.Update(dt);
		
		int mouse_x, mouse_y;
		GetMousePos(mouse_x, mouse_y);
				
		vector transform[4];
		m_EditorObject.GetWorldObject().GetTransform(transform);
		
		vector screen_position = GetGame().GetScreenPos(m_Offset.Multiply4(transform));

		float x, y, w, h;		
		m_LayoutRoot.GetPos(x, y);
		m_LayoutRoot.GetSize(w, h);
		
		bool is_in_bound = (mouse_x < x + h && mouse_x > x - h) && (mouse_y < y + h && mouse_y > y - h);
		m_LayoutRoot.SetAlpha(is_in_bound);
		
		m_LayoutRoot.SetPos(screen_position[0] - w / 2, screen_position[1] - h / 2);
		m_LayoutRoot.Show(screen_position[2] > 0);
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
	
	void CheckForDragging()
	{
		
	}
		
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		switch (button) {
			case MouseState.LEFT: {
				if (KeyState(KeyCode.KC_LCONTROL)) {
					m_EditorObject.SetSelected(!m_EditorObject.IsSelected());
					return true;
				}
				
				if (!KeyState(KeyCode.KC_LSHIFT)) {
					m_EditorObject.ClearSelections();
				}
				
				m_EditorObject.SetSelected(true);
								
				return true;
			}
			
			case MouseState.MIDDLE: {
				/*
				EditorCamera camera = GetEditor().GetCamera();
				vector pos = m_EditorObject.GetWorldObject().GetPosition();
				pos[1] = camera.GetPosition()[1];
				camera.SendToPosition(pos);*/
				return true;
			}
			
			case MouseState.RIGHT: {
				/*
				if (!m_EditorObject.IsSelected() && !KeyState(KeyCode.KC_LSHIFT)) {
					EditorObject.ClearSelections();
				}
				
				m_EditorObject.SetSelected(true);
				
				if (EditorHud.CurrentMenu) {
					delete EditorHud.CurrentMenu;
				}
				
				EditorHud.CurrentMenu = new EditorPlacedContextMenu(x, y, m_EditorObject);
				*/
				return true;
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w) {
			case m_LayoutRoot: {
				//WidgetAnimator.Animate(m_LayoutRoot, WidgetAnimatorProperty.COLOR_A, 1.0, 100);
				break;
			}
			
			case Image: {
				WidgetAnimator.AnimateColor(Image, ARGB(255, 102, 189, 181), 100);
				break;
			}
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w) {
			case m_LayoutRoot: {
				//WidgetAnimator.Animate(m_LayoutRoot, WidgetAnimatorProperty.COLOR_A, 0.0, 100);
				break;
			}
			
			case Image: {
				WidgetAnimator.AnimateColor(Image, ARGB(255, 255, 255, 255), 100);
				break;
			}
		}
			
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\EditorPoint.layout";
	}
}