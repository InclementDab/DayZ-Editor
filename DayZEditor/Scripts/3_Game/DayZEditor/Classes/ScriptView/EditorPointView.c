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
				
		vector transform[4];
		m_EditorObject.GetWorldObject().GetTransform(transform);
		
		vector screen_position = GetGame().GetScreenPos(m_Offset.Multiply4(transform));
		if (m_LayoutRoot) {
			float w, h;
			m_LayoutRoot.GetSize(w, h);
			m_LayoutRoot.SetPos(screen_position[0] - w / 2, screen_position[1] - h / 2);
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
	
	void CheckForDragging()
	{
		
	}
		
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		switch (button) {
			
			case MouseState.LEFT: {
				m_EditorObject.SetSelected(true);
				return false;
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
		WidgetAnimator.AnimateColor(Image, ARGB(255, 102, 189, 181), 100);
		
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