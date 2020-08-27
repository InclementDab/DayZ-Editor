

class EditorDialogController: Controller
{
	
	Widget TitleBar;
	TextWidget TitleText;
	ButtonWidget TitleClose;
	
	WrapSpacerWidget DialogContent;
	GridSpacerWidget ButtonGrid;
	WrapSpacerWidget WindowDragWrapper;
	
	private ref ButtonCallbackHashMap m_ButtonCallbacks = new ButtonCallbackHashMap();
	
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
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		if (w == TitleBar) {

			m_LayoutRoot.SetPos(x - m_OffsetX, y - m_OffsetY, true);
			TitleBar.SetPos(0, 0, true);
			return true;
		}
		
		return true;//super.OnDragging(w, x, y, reciever);
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
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
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorDialogController::OnClick");
		
		if (button != 0) return false; 
		
		if (w == TitleClose) {
			m_EditorDialog.CloseDialog();
			return true;
		}
		

		if (m_ButtonCallbacks[w] != string.Empty) {
			GetGame().GameScript.Call(m_EditorDialog, m_ButtonCallbacks[w], null);
			return true;
		}
		
		return false;
	}
	
	void AddButton(ButtonWidget bw, string callback)
	{
		m_ButtonCallbacks.Insert(bw, callback);
	}
}


typedef ref map<ButtonWidget, string> ButtonCallbackHashMap;

class EditorDialog: Managed
{
	
	
	protected ref Widget m_LayoutRoot;
	Widget GetRoot() { 
		return m_LayoutRoot; 
	}
	
	protected ref EditorDialogController m_DialogController;
	
	
	void EditorDialog() 
	{
		EditorLog.Trace("EditorDialog");

		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorDialog.layout", null);
		m_LayoutRoot.GetScript(m_DialogController);
		m_LayoutRoot.Show(false);
		
		m_DialogController.SetEditorDialog(this);
	}
	
	void ~EditorDialog() 
	{
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
		ButtonWidget bw = ButtonWidget.Cast(panel.FindAnyWidget("Button"));
		bw.SetText(label);
		m_DialogController.AddButton(bw, callback);
		return bw;
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
