class EditorDialogController: Controller
{
	Widget TitleBar;
	TextWidget TitleText;
	ButtonWidget TitleClose;
	
	WrapSpacerWidget DialogContent;
	GridSpacerWidget ButtonGrid;
	WrapSpacerWidget WindowDragWrapper;
	
	// todo move into Controller as an array, and then execute all the events using super.XXXX
	protected Controller m_ChildController;
	void SetChildController(Controller child_controller) {
		EditorLog.Trace("EditorDialogController::SetChildController");
		m_ChildController = child_controller;
	}
	
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
		
		m_ChildController.OnDrag(target, x, y);
	}
	
	override void OnDragging(Widget target, int x, int y)
	{
		EditorLog.Trace("EditorDialogController::OnDragging: %1 X:%2 Y:%3", target.GetName(), x.ToString(), y.ToString());
		m_LayoutRoot.SetPos(x + m_OffsetX, y + m_OffsetY);		
		
		m_ChildController.OnDragging(target, x, y);
	}
	
	override void OnDrop(Widget target, Widget drop_target, int x, int y)
	{
		EditorLog.Trace("EditorDialogController::OnDrop");
	    if (target == WindowDragWrapper) {
			m_LayoutRoot.SetPos(x + m_OffsetX, y + m_OffsetY);
			return;
	    }
		
	    m_ChildController.OnDrop(target, drop_target, x, y);
	}
	
	override void OnClick(Widget w, int button, int x, int y)
	{
		EditorLog.Trace("EditorDialogController::OnClick");
		
		if (button != 0) return; 
		
		if (w == TitleClose) {
			m_EditorDialog.CloseDialog();
			return;
		}
		
		m_ChildController.OnClick(w, button, x, y);

	}
	
}
