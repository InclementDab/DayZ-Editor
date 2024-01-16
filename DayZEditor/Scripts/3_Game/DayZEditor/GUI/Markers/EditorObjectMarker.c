class EditorObjectMarker: EditorMarker
{	
	protected EditorObject m_EditorObject;

	protected ref EditorDragHandler m_DragHandler;
	
	void EditorObjectMarker(EditorObject editor_object)
	{
		EditorLog.Trace("EditorObjectMarker");
		m_EditorObject = editor_object;
		
		//m_EditorObject.OnObjectSelected.Insert(EditorObjectSelected);
		//m_EditorObject.OnObjectDeselected.Insert(EditorObjectDeselected);	
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
		if (show && m_EditorObject) {
			return;
		}
		
		super.Show(show);
	}
	
	void Select() 
	{
		m_LayoutRoot.SetAlpha(MARKER_ALPHA_ON_SHOW);
		SetColor(m_Editor.GeneralSettings.SelectionColor);
		SetOutlineColor(m_Editor.GeneralSettings.MarkerPrimaryColor);
	}
	
	void Highlight()
	{
		m_LayoutRoot.SetAlpha(MARKER_ALPHA_ON_SHOW);
		SetColor(m_Editor.GeneralSettings.MarkerPrimaryColor);
		SetOutlineColor(m_Editor.GeneralSettings.HighlightColor);
	}
	
	void Deselect() 
	{
		m_LayoutRoot.SetAlpha(MARKER_ALPHA_ON_HIDE);
		SetColor(m_Editor.GeneralSettings.MarkerPrimaryColor);
		SetOutlineColor(m_Editor.GeneralSettings.MarkerPrimaryColor);
	}
	
	bool IsSelected() 
	{
		return m_EditorObject.IsSelected();
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (!IsSelected()) {
			Highlight();
			//if (m_EditorObject.GetListItem()) {
			//	m_EditorObject.GetListItem().Highlight();
			//}
		}
		
		if (m_Editor.GeneralSettings.MarkerTooltips && !m_Editor.IsPlacing()) {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DoTooltipCheck, 500);
		}
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (!IsSelected()) {
			Deselect();
			//if (m_EditorObject.GetListItem()) {
			//	m_EditorObject.GetListItem().Deselect();
			//}
		}
		
		if (m_Editor.GeneralSettings.MarkerTooltips) {
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
			GetEditor().GetEditorHud().SetCurrentTooltip(EditorTooltip.CreateOnButton(m_EditorObject.GetWorldObject().GetType(), GetLayoutRoot(), TooltipPositions.BOTTOM_LEFT));
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
			
			if (dist_x + dist_y > DRAG_THRESHOLD) {
				m_EditorObject.SetSelected(true);
				m_DragHandler.OnDragStart();
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