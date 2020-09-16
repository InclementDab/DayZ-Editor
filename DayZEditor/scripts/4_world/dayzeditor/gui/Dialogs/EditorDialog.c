
class EditorDialog: EditorMVCLayout
{	
	protected ref EditorDialogController m_DialogController;
	EditorDialogController GetDialogController() {
		return EditorDialogController.Cast(m_Controller);
	}
	
	string GetTitle() {
		return m_Title;
	}
	
	void EditorDialog() 
	{
		EditorLog.Trace("EditorDialog");
		
		m_LayoutRoot.Show(false);
		
		m_DialogController = GetDialogController();
		if (m_DialogController) {
			m_DialogController.SetEditorDialog(this);
		}
	}
	
	void ~EditorDialog() 
	{
		EditorLog.Trace("~EditorDialog");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}

	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/EditorDialog.layout";
	}
	
	override typename GetControllerType() {
		return EditorDialogController;
	}
	
	protected Controller AddContent(Widget content)
	{
		if (!m_DialogController) return m_DialogController;
		m_DialogController.DialogContent.AddChild(content);
		Controller controller;
		content.GetScript(controller);		
		return controller;
	}
	
	protected Controller AddContent(string layout)
	{
		if (!m_DialogController) return m_DialogController;
		Widget content = GetGame().GetWorkspace().CreateWidgets(layout);
		m_DialogController.DialogContent.AddChild(content);
		Controller controller;
		content.GetScript(controller);		
		return controller;
	}
	
	protected Controller AddContent(EditorPrefab prefab)
	{
		if (!m_DialogController) return m_DialogController;
		m_DialogController.DialogContent.AddChild(prefab.GetLayoutRoot());
		Controller controller;
		prefab.GetLayoutRoot().GetScript(controller);
		return controller;
	}
	
	
	protected ButtonWidget AddButton(string label, string callback) 
	{
		Widget panel = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorDialogButton.layout", m_DialogController.ButtonGrid);		
		EditorDialogButtonHandler handler;
		panel.GetScript(handler);
		handler.SetLabel(label);
		handler.SetCallback(this, callback);
		return panel.FindAnyWidget("DialogButton");
	}
	
	
	private string m_Title;
	protected void SetTitle(string title)
	{
		m_Title = title;
		m_DialogController.TitleText.SetText(m_Title);
	}
		
	
	override void Show()
	{
		EditorLog.Trace("EditorDialog::Show");
		super.Show();
				
		m_Editor.GetCamera().SetMoveEnabled(false);
		m_Editor.GetCamera().SetLookEnabled(false);
		m_EditorHud.ShowCursor();
		
		m_EditorHud.SetModal(this);
		
		float du, dv, dx, dy;
		m_LayoutRoot.GetScreenSize(du, dv);		
		m_LayoutRoot.GetPos(dx, dy);
		m_LayoutRoot.SetPos(dx, dy - dv / 2);
	}
	
	override void Close()
	{
		EditorLog.Trace("EditorDialog::Close");
		super.Close();
		
		m_EditorHud.SetModal(null);
		m_Editor.GetCamera().SetMoveEnabled(true);
		m_Editor.GetCamera().SetLookEnabled(true);
		m_EditorHud.ShowCursor();
	}
	
	void Update();

	
}
