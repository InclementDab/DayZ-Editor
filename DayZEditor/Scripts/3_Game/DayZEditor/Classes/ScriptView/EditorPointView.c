class EditorPointView: ScriptView
{	
	protected EditorObject m_EditorObject;
	protected vector m_Offset;
	protected float m_CameraDistance;
	protected bool m_IsDragging;
	protected float m_PlacementDistance;
	
	ImageWidget Image;
	
	void EditorPointView(notnull EditorObject editor_object, vector offset, float camera_distance)
	{
		m_EditorObject = editor_object;
		m_Offset = offset;
		m_CameraDistance = camera_distance;
	}
	
	override void Update(float dt)
	{
		super.Update(dt);
		
		if ((GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK)  != MB_PRESSED_MASK) {
			m_IsDragging = false;
		}
		
		int mouse_x, mouse_y;
		GetMousePos(mouse_x, mouse_y);
		
		vector transform[4];
		m_EditorObject.GetWorldObject().GetTransform(transform);
		
		if (m_IsDragging) {
			EditorObjectDragHandler.Drag(m_EditorObject, m_PlacementDistance, m_Offset);
		}

		float x, y, w, h;		
		m_LayoutRoot.GetPos(x, y);
		m_LayoutRoot.GetSize(w, h);
		

		vector transform_pos = m_Offset.Multiply4(transform);
		vector screen_position = GetGame().GetScreenPos(transform_pos);
		
		bool is_in_bound = (mouse_x < x + h && mouse_x > x - h) && (mouse_y < y + h && mouse_y > y - h);
		//Print(vector.Distance(transform_pos, GetGame().GetCurrentCameraPosition()));
		m_LayoutRoot.SetAlpha(is_in_bound || !GetGame().GetUIManager().IsCursorVisible() || m_EditorObject.IsSelected());
		
		m_LayoutRoot.SetPos(screen_position[0] - w / 2, screen_position[1] - h / 2);
		m_LayoutRoot.Show(screen_position[2] > 0 && (vector.Distance(transform_pos, GetGame().GetCurrentCameraPosition()) < m_CameraDistance));
	}

	protected void CheckForDragging()
	{
		if ((GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) == MB_PRESSED_MASK) {
			m_IsDragging = true;
			
			vector position;	
			vector end_pos = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * 3000;
			int interaction_layers = PhxInteractionLayers.BUILDING | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.DYNAMICITEM | PhxInteractionLayers.ITEM_LARGE;
			Object hit_object;
			vector normal;
			float fraction;
			DayZPhysics.RayCastBullet(GetGame().GetCurrentCameraPosition(), end_pos, interaction_layers, null, hit_object, position, normal, fraction);
			m_PlacementDistance = vector.Distance(position, GetGame().GetCurrentCameraPosition());
		}
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
				
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(CheckForDragging, 100);
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
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		switch (button) {
			case MouseState.LEFT: {
				m_IsDragging = false;
				break;
			}
		}
		
		return super.OnMouseButtonUp(w, x, y, button);
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