

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
	bool OnDrag(Widget w, int x, int y) 
	{
		EditorLog.Trace("EditorDialogController::OnDrag");
		if (w == TitleBar) {
			
			m_LayoutRoot.GetPos(m_OffsetX, m_OffsetY);
			
			m_OffsetX = x - m_OffsetX;
			m_OffsetY = y - m_OffsetY;
	
			TitleBar.SetPos(0, 0, true);
			TitleBar.SetPos(0, 0, false);
			
			return true;
		}
		
		return false;
	}
	
	bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		if (w == TitleBar) {

			m_LayoutRoot.SetPos(x - m_OffsetX, y - m_OffsetY, true);
			TitleBar.SetPos(0, 0, true);
			return true;
		}
		
		return true;//super.OnDragging(w, x, y, reciever);
	}
	
	bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		EditorLog.Trace("EditorDialogController::OnDrop");
	    if (w == TitleBar) {
	        TitleBar.SetPos(0, 0);
			m_LayoutRoot.SetPos(x - m_OffsetX, y - m_OffsetY);
			WindowDragWrapper.SetPos(x - m_OffsetX, y - m_OffsetY);
	        return true;
	    }
	    
	    return false;
	}
	
	override void OnClick(Widget w, int button, int x, int y)
	{
		EditorLog.Trace("EditorDialogController::OnClick");
		
		if (button != 0) return; 
		
		if (w == TitleClose) {
			m_EditorDialog.CloseDialog();
		}

	}
	
}


class EditorDialogButtonHandler: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	
	protected Widget DialogButtonOutline;
	protected ButtonWidget DialogButton;
	
	protected Class m_CallbackContext;
	protected string m_CallbackAction;

	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("EditorDialogButtonHandler::OnWidgetScriptInit");
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
	}
	
	void SetLabel(string label) {
		DialogButton.SetText(label);
	}
	
	void SetCallback(Class context, string action) {
		m_CallbackContext = context;
		m_CallbackAction = action;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{	
		EditorLog.Trace("EditorDialogButtonHandler::OnClick");
		if (button != 0) return false;
		
		if (m_CallbackContext && m_CallbackAction) {
			g_Script.Call(m_CallbackContext, m_CallbackAction, null);
			return true;
		}
				
		return false;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorDialogButtonHandler::OnMouseEnter");
		
		
		
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		EditorLog.Trace("EditorDialogButtonHandler::OnMouseLeave");
		
		
		return false;
	}
	
	
	
}


class EditorDialog: Managed
{
	
	
	protected ref Widget m_LayoutRoot;
	Widget GetRoot() { 
		return m_LayoutRoot; 
	}
	
	protected ref EditorDialogController m_DialogController;
	Controller GetController() { 
		return m_DialogController; 
	}
	
	
	void EditorDialog() 
	{
		EditorLog.Trace("EditorDialog");

		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorDialog.layout", null);
		m_LayoutRoot.GetScript(m_DialogController);
		m_LayoutRoot.Show(false);
		
		m_DialogController.SetEditorDialog(this);
	}
	
	void ~EditorDialog() {
		EditorLog.Trace("~EditorDialog");
	}


	protected void AddContent(Widget content)
	{
		Print(m_DialogController);
		m_DialogController.DialogContent.AddChild(content);
	}
	
	
	protected ButtonWidget AddButton(string label, string callback) 
	{
		Widget panel = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorDialogButton.layout", m_DialogController.ButtonGrid);		
		EditorDialogButtonHandler handler;
		panel.GetScript(handler);
		handler.SetLabel(label);
		handler.SetCallback(this, callback);
		return panel;
	}
	
	
	protected void SetTitle(string title)
	{
		m_DialogController.TitleText.SetText(title);
	}
	
	void ShowDialog()
	{
		EditorLog.Trace("EditorDialog::ShowDialog");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
		m_LayoutRoot.Show(true);
		GetEditor().GetEditorHud().ModalSet(this);
		
		float du, dv, dx, dy;
		m_LayoutRoot.GetScreenSize(du, dv);		
		m_LayoutRoot.GetPos(dx, dy);
		m_LayoutRoot.SetPos(dx, dy - dv / 2);
	}
	
	void CloseDialog()
	{
		EditorLog.Trace("EditorDialog::CloseDialog");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		m_LayoutRoot.Show(false);
		GetEditor().GetEditorHud().ModalClose();
	}
	
	void Update();

	
}
