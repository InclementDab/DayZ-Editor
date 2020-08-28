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
	
	override void PropertyChanged(string property_name)
	{
		Print("PropertyChanged " + property_name);
	}
	
	
	float m_OffsetX, m_OffsetY;
	override bool OnDrag(Widget w, int x, int y) 
	{
		EditorLog.Trace("EditorDialogController::OnDrag");
		if (w == WindowDragWrapper) {
			m_LayoutRoot.GetPos(m_OffsetX, m_OffsetY);
			m_OffsetX -= x; m_OffsetY -= y;
		}
		
		return false;
		
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		EditorLog.Trace("EditorDialogController::OnDragging: %1 X:%2 Y:%3", w.GetName(), x.ToString(), y.ToString());
		m_LayoutRoot.SetPos(x + m_OffsetX, y + m_OffsetY);
		return false;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		EditorLog.Trace("EditorDialogController::OnDrop");
	    if (w == WindowDragWrapper) {
			m_LayoutRoot.SetPos(x + m_OffsetX, y + m_OffsetY);
			return false;
		}
		
		return false;
	}
	
	override void OnClick(ViewBinding target, int button, int x, int y)
	{
		EditorLog.Trace("EditorDialogController::OnClick");
		if (button != 0) return; 
		
		if (target.GetRoot() == TitleClose) {
			m_EditorDialog.CloseDialog();
			return;
		}
		
	}
	
}
