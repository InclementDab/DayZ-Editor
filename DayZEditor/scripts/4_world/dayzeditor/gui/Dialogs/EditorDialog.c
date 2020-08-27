


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
		m_DialogController.DialogContent.AddChild(content);
	}
	
	protected Controller AddContentWithController(Widget content)
	{
		m_DialogController.DialogContent.AddChild(content);
		Controller controller;
		content.GetScript(controller);
		if (controller) {
			m_DialogController.SetChildController(controller);
		}
		
		return controller;
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
