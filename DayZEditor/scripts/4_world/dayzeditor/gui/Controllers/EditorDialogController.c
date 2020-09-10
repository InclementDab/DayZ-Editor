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
	
	
/*	
	float m_OffsetX, m_OffsetY;
	override void MVCOnDrag(Widget target, int x, int y) 
	{
		EditorLog.Trace("EditorDialogController::OnDrag");
		if (target == WindowDragWrapper) {
			m_LayoutRoot.GetPos(m_OffsetX, m_OffsetY);
			m_OffsetX -= x; m_OffsetY -= y;
		}
		
	}
	
	override void MVCOnDragging(Widget target, int x, int y)
	{
		EditorLog.Trace("EditorDialogController::OnDragging: %1 X:%2 Y:%3", target.GetName(), x.ToString(), y.ToString());
		m_LayoutRoot.SetPos(x + m_OffsetX, y + m_OffsetY);
	}
	
	override void MVCOnDrop(Widget target, Widget drop_target, int x, int y)
	{
		EditorLog.Trace("EditorDialogController::OnDrop");
	    if (target == WindowDragWrapper) {
			m_LayoutRoot.SetPos(x + m_OffsetX, y + m_OffsetY);
			return;
	    }
		
	}
	
	override void MVCOnClick(Widget target, int button, int x, int y)
	{
		EditorLog.Trace("EditorDialogController::OnClick");
		
		if (button != 0) return; 
		
		if (target == TitleClose) {
			m_EditorDialog.CloseDialog();
			return;
		}
		
	}
	*/
}
