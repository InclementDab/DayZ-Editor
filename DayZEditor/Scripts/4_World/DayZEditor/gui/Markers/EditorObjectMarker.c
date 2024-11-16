class EditorObjectMarker: EditorMarker
{		
	protected EditorObject m_EditorObject;
	
	void EditorObjectMarker(EditorObject editor_object)
	{
		EditorLog.Trace("EditorObjectMarker");
		m_EditorObject = editor_object;
				
		m_EditorObject.OnObjectSelected.Insert(EditorObjectSelected);
		m_EditorObject.OnObjectDeselected.Insert(EditorObjectDeselected);	
	}
	
	void EditorObjectSelected(EditorObject data) 
	{		
		SetHighlighted(2);
	}
	
	void EditorObjectDeselected(EditorObject data) 
	{
		SetHighlighted(0);
	}
		
	override void Show(bool show)
	{
		// dont show if locked
		if (show && m_EditorObject && m_EditorObject.Locked) {
			return;
		}
		
		super.Show(show);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{	
		// ignores the object if you are placing
		if (m_Editor.IsPlacing()) { 
			return false;
		}

		// Delete the current tooltip to clean the UI a bit
		GetEditor().GetEditorHud().SetCurrentTooltip(null);

		switch (button) {
			case MouseState.LEFT: {
				
				if (!m_Editor.CanSelectObject()) {
					return true;
				}
				
				// We want to Toggle selection if you are holding control
				if (KeyState(KeyCode.KC_LCONTROL)) {
					m_Editor.ToggleSelection(m_EditorObject);
					return true;
				} 
				
				// allows multiple objects to be dragged
				if (m_EditorObject.IsSelected()) {
					thread CheckDragBounds(x, y);
					return true;
				}
				
				if (!KeyState(KeyCode.KC_LSHIFT)) {
					m_Editor.ClearSelection();
				}
				
				m_Editor.SelectObject(m_EditorObject);
				
				thread CheckDragBounds(x, y);
				return false;
			}
			
			case MouseState.MIDDLE: {
				EditorCamera camera = GetEditor().GetCamera();

				vector camera_transform[4];
				camera.GetTransform(camera_transform);
				Math3D.MatrixInverse3(camera_transform);
				camera_transform[3] = m_EditorObject.GetPosition();

				vector new_position = Vector(4.0, 4.0, 4.0).Multiply4(camera_transform);
				camera.SetPosition(new_position);
				camera.LookAt(m_EditorObject.GetPosition());
				return true;
			}
		}

		return true;
	}
		
	override bool OnPress(Widget w, int x, int y, int button)
	{		
		// ignores the object if you are placing
		if (m_Editor.IsPlacing()) { 
			return false;
		}
		
		// Delete the current tooltip to clean the UI a bit
		GetEditor().GetEditorHud().SetCurrentTooltip(null);
		
		switch (button) {			
			case MouseState.RIGHT: {
				
				if (!m_EditorObject.IsSelected() && !KeyState(KeyCode.KC_LSHIFT)) {
					m_Editor.ClearSelection();
				}
				
				m_Editor.SelectObject(m_EditorObject);
				
				if (EditorHud.CurrentMenu) {
					delete EditorHud.CurrentMenu;
				}
				
				EditorHud.CurrentMenu = new EditorPlacedContextMenu(x, y, m_EditorObject);
				return true;
			}
		}
		
		return super.OnPress(w, x, y, button);
	}
				
	override bool IsDisabled()
	{
		return m_EditorObject && (m_EditorObject.GetFlags() & (EditorObjectFlags.NOSAVE | EditorObjectFlags.NODELETE));		
	}
	
	bool IsSelected() 
	{
		return m_EditorObject.IsSelected();
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (!IsSelected()) {
			SetHighlighted(1);
			if (m_EditorObject.GetListItem()) {
				m_EditorObject.GetListItem().Highlight();
			}
		}
		
		if (m_Editor.GetSettings().MarkerTooltips && !m_Editor.IsPlacing()) {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DoTooltipCheck, 500);
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (!IsSelected()) {
			SetHighlighted(0);
			if (m_EditorObject.GetListItem()) {
				m_EditorObject.GetListItem().Deselect();
			}
		}
		
		if (m_Editor.GetSettings().MarkerTooltips) {
			GetEditor().GetEditorHud().SetCurrentTooltip(null);
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	private void DoTooltipCheck()
	{
		int x, y;
		GetMousePos(x, y);
		if (!IsMouseInside(x, y)) {
			return;
		}
		
		if (!EditorHud.CurrentMenu) {
			GetEditor().GetEditorHud().SetCurrentTooltip(EditorTooltip.CreateOnButton(m_EditorObject.GetType(), GetLayoutRoot(), TooltipPositions.BOTTOM_LEFT, string.Format("(%1)", m_EditorObject.GetID())));
		}
	}
	
	private const int DRAG_THRESHOLD = 5;
	private void CheckDragBounds(int x, int y)
	{
		while (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) {
			int c_x, c_y;
			GetMousePos(c_x, c_y);
			
			int dist_x = Math.AbsInt(x - c_x);
			int dist_y = Math.AbsInt(y - c_y);
			
			if (Math.Sqrt(dist_x * dist_x + dist_y * dist_y) > DRAG_THRESHOLD) {
				m_Editor.SelectObject(m_EditorObject);
				
				array<EditorObject> additional_drag_targets = m_Editor.GetSelectedObjects().GetValueArray();
				additional_drag_targets.RemoveItem(m_EditorObject);
				GetEditor().DragHandler = EditorDragHandler.Cast(GetDragHandlerType().Spawn());
				GetEditor().DragHandler.OnDragStart(m_EditorObject, additional_drag_targets);
				return;
			}
			
			Sleep(10);
		}
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		m_Editor.CommandManager[EditorObjectPropertiesCommand].Execute(this, null);
		return true;
	}
	
	EditorObject GetEditorObject() 
	{ 
		return m_EditorObject; 
	}

	typename GetDragHandlerType()
	{
		return EditorDragHandler;
	}
}