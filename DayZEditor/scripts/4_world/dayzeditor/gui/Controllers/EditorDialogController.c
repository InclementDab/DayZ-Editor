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
	override void OnDrag(Widget target, int x, int y) 
	{
		EditorLog.Trace("EditorDialogController::OnDrag");
		if (target == WindowDragWrapper) {
			m_LayoutRoot.GetPos(m_OffsetX, m_OffsetY);
			m_OffsetX -= x; m_OffsetY -= y;
		}
		
		super.OnDrag(target, x, y);
	}
	
	override void OnDragging(Widget target, int x, int y)
	{
		EditorLog.Trace("EditorDialogController::OnDragging: %1 X:%2 Y:%3", target.GetName(), x.ToString(), y.ToString());
		m_LayoutRoot.SetPos(x + m_OffsetX, y + m_OffsetY);		
		
		super.OnDragging(target, x, y);
	}
	
	override void OnDrop(Widget target, Widget drop_target, int x, int y)
	{
		EditorLog.Trace("EditorDialogController::OnDrop");
	    if (target == WindowDragWrapper) {
			m_LayoutRoot.SetPos(x + m_OffsetX, y + m_OffsetY);
			return;
	    }
		
	    super.OnDrop(target, drop_target, x, y);
	}
	
	override void OnClick(Widget w, int button, int x, int y)
	{
		EditorLog.Trace("EditorDialogController::OnClick");
		
		if (button != 0) return; 
		
		if (w == TitleClose) {
			m_EditorDialog.CloseDialog();
			return;
		}
		
		super.OnClick(w, button, x, y);

	}
	
}
