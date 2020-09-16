class EditorDialogController: Controller
{
	Widget TitleBar;
	TextWidget TitleText;
	ButtonWidget TitleClose;
	
	WrapSpacerWidget DialogContent;
	GridSpacerWidget ButtonGrid;
	WrapSpacerWidget WindowDragWrapper;
	
	protected ref EditorDialog m_EditorDialog;
	void SetEditorDialog(EditorDialog editor_dialog) {
		m_EditorDialog = editor_dialog;
	}
	
	void EditorDialogController() {
		EditorLog.Trace("EditorDialogController");
	}
	
	void ~EditorDialogController() {
		EditorLog.Trace("~EditorDialogController");
	}
	
	float m_OffsetX, m_OffsetY;
	override bool OnDrag(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorDialogController::OnDrag");
		if (w == WindowDragWrapper) {
			m_LayoutRoot.GetPos(m_OffsetX, m_OffsetY);
			m_OffsetX -= x; m_OffsetY -= y;		
			g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Insert(DragUpdate);	
		}
		
		return false;
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever) { return true; }
	
	private bool DragUpdate()
	{
		int x, y;
		GetMousePos(x, y);
		m_LayoutRoot.SetPos(x + m_OffsetX, y + m_OffsetY);
		return false;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		EditorLog.Trace("EditorDialogController::OnDrop");
		g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Remove(DragUpdate);
		
		if (w == WindowDragWrapper) {
			m_LayoutRoot.SetPos(x + m_OffsetX, y + m_OffsetY);
	    }		
		
		return false;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorDialogController::OnClick");
		
		if (button != 0) return false; 
		
		if (w == TitleClose) {
			m_EditorDialog.Close();
		}
		
		return false;
	}
	
}
