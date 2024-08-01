class EditorObjectMarker: EditorMarker
{	
	protected EditorObject m_EditorObject;

	protected ref EditorDragHandler m_DragHandler;
	
	void EditorObjectMarker(EditorObject editor_object)
	{
		EditorLog.Trace("EditorObjectMarker");
		m_EditorObject = editor_object;
		
		m_EditorObject.OnObjectSelected.Insert(EditorObjectSelected);
		m_EditorObject.OnObjectDeselected.Insert(EditorObjectDeselected);	
	}
	
	void ~EditorObjectMarker()
	{
		delete m_DragHandler;
	}
	
	void EditorObjectSelected(EditorObject data) 
	{		
		Select();
	}
	
	void EditorObjectDeselected(EditorObject data) 
	{
		Deselect();
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
				vector pos = m_EditorObject.GetPosition();
				pos[1] = camera.GetPosition()[1];
				camera.SendToPosition(pos);
				return true;
			}
			
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
		
		return super.OnMouseButtonDown(w, x, y, button);
	}

	void Select() 
	{
		m_LayoutRoot.SetAlpha(MARKER_ALPHA_ON_SHOW);
		SetColor(m_Editor.Settings.SelectionColor);
		SetOutlineColor(m_Editor.Settings.MarkerPrimaryColor);
	}
	
	void Highlight()
	{
		m_LayoutRoot.SetAlpha(MARKER_ALPHA_ON_SHOW);
		SetColor(m_Editor.Settings.MarkerPrimaryColor);
		SetOutlineColor(m_Editor.Settings.HighlightColor);
	}
	
	void Deselect() 
	{
		m_LayoutRoot.SetAlpha(MARKER_ALPHA_ON_HIDE);
		SetColor(m_Editor.Settings.MarkerPrimaryColor);
		SetOutlineColor(m_Editor.Settings.MarkerPrimaryColor);
	}
	
	bool IsSelected() 
	{
		return m_EditorObject.IsSelected();
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (!IsSelected()) {
			Highlight();
			if (m_EditorObject.GetListItem()) {
				m_EditorObject.GetListItem().Highlight();
			}
		}
		
		if (m_Editor.Settings.MarkerTooltips && !m_Editor.IsPlacing()) {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DoTooltipCheck, 500);
		}
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (!IsSelected()) {
			Deselect();
			if (m_EditorObject.GetListItem()) {
				m_EditorObject.GetListItem().Deselect();
			}
		}
		
		if (m_Editor.Settings.MarkerTooltips) {
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
				m_DragHandler.OnDragStart(m_EditorObject, additional_drag_targets);
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
	
	EditorDragHandler GetDragHandler() 
	{
		return m_DragHandler; 
	}
}