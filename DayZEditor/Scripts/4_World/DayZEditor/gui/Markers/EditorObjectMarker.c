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
		
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{	
		// ignores the object if you are placing
		if (m_Editor.IsPlacing() || m_Editor.Brush) { 
			return false;
		}
		
		// Delete the current tooltip to clean the UI a bit
		GetEditor().GetEditorHud().SetCurrentTooltip(null);

		switch (button) {
			case MouseState.LEFT: {
				if (!m_EditorObject.IsSelected() && !GetEditor().IsShiftDown() && !GetEditor().IsCtrlDown()) {
					m_Editor.ClearSelection();
				}
				
				CheckDragBounds(x, y);
				//SetHighlighted(2);
				break;
			}
		}

		return super.OnMouseButtonDown(w, x, y, button);
	}
		
	override bool OnPress(Widget w, int x, int y, int button)
	{		
		// ignores the object if you are placing
		if (m_Editor.IsPlacing() || m_Editor.Brush) { 
			return false;
		}
				
		// Delete the current tooltip to clean the UI a bit
		GetEditor().GetEditorHud().SetCurrentTooltip(null);
		
		switch (button) {			
			case MouseState.RIGHT: {
				
				if (!m_EditorObject.IsSelected() && !GetEditor().IsShiftDown()) {
					m_Editor.ClearSelection();
				}
				
				m_Editor.SelectObject(m_EditorObject);
				
				if (EditorHud.CurrentMenu) {
					delete EditorHud.CurrentMenu;
				}
				
				EditorHud.CurrentMenu = new EditorPlacedContextMenu(x, y, m_EditorObject);
				return true;
			}
						
			case MouseState.MIDDLE:
			case MouseState.LEFT: {			
				if (button == MouseState.MIDDLE) {
					vector clip_info[2];
					m_EditorObject.ClippingInfo(clip_info);
					
					vector transform[4];
					m_EditorObject.GetTransform(transform);
					
					vector high_point = 2 * Vector(Math.Max(clip_info[0][0], clip_info[1][0]), Math.Max(clip_info[0][1], clip_info[1][1]), Math.Max(clip_info[0][2], clip_info[1][2]));
					high_point[1] = (high_point[0] + high_point[2]) * 0.5;					
					vector new_position = high_point.Multiply4(transform);
					
					EditorCamera camera = GetEditor().GetCamera();
					camera.SetPosition(new_position);
					camera.LookAt(m_EditorObject.GetBottomCenter());
				}
				
				if (GetEditor().IsDragging()) {
					return true;
				}
				
				if (GetEditor().IsCtrlDown()) {
					GetEditor().ToggleSelection(m_EditorObject);
					return true;
				}
				
				if (!GetEditor().IsShiftDown()) {
					GetEditor().ClearSelection();
				}
				
				GetEditor().SelectObject(m_EditorObject);		
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
		// ignores the object if you are placing
		if (m_Editor.IsPlacing() || m_Editor.Brush) { 
			return false;
		}
				
		if (!IsSelected()) {
			SetHighlighted(1);
			if (m_EditorObject.GetListItem()) {
				m_EditorObject.GetListItem().Highlight();
			}
		}
		
		if (m_Editor.GetSettings().MarkerTooltips && !m_Editor.IsPlacing()) {
			string description = string.Format("(%1)", m_EditorObject.GetModelPath());
			GetEditor().GetEditorHud().CreateDelayedTooltip(w, m_EditorObject.GetDisplayName(), TooltipPosition.INSIDE, "", Symbols.HOUSE);
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
		
		GetEditor().GetEditorHud().SetCurrentTooltip(null);
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
		
	private const int DRAG_THRESHOLD_SQ = 25;
	private void CheckDragBounds(int x, int y)
	{
		if (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) {
			int c_x, c_y;
			GetMousePos(c_x, c_y);
			
			int dist_x = Math.AbsInt(x - c_x);
			int dist_y = Math.AbsInt(y - c_y);
			
			if (dist_x * dist_x + dist_y * dist_y > DRAG_THRESHOLD_SQ) {
				m_Editor.SelectObject(m_EditorObject);
				
				array<EditorObject> additional_drag_targets = m_Editor.GetSelectedObjects().GetValueArray();
				additional_drag_targets.RemoveItem(m_EditorObject);
				GetEditor().DragHandler = EditorDragHandler.Cast(GetDragHandlerType().Spawn());
				GetEditor().DragHandler.OnDragStart(m_EditorObject, additional_drag_targets);
				return;
			}
			
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(CheckDragBounds, 10, false, x, y);
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