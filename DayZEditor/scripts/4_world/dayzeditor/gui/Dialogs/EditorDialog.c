


class EditorDialog: MVCWidgetHandler
{
	private Editor m_Editor;
	private EditorHud m_EditorHud;
	
	protected ref EditorDialogController m_DialogController;
	Controller GetController() {
		return m_DialogController;
	}
	
	string GetTitle() {
		return m_Title;
	}
	
	void EditorDialog() {
		
		EditorLog.Trace("EditorDialog");

		m_Editor = GetEditor();
		m_EditorHud = m_Editor.GetEditorHud();
		
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorDialog.layout", null);
		m_LayoutRoot.GetScript(m_DialogController);
		m_LayoutRoot.Show(false);
		
		m_DialogController.SetEditorDialog(this);
	}
	
	void ~EditorDialog() {
		EditorLog.Trace("~EditorDialog");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}


	
	protected Controller AddContent(Widget content)
	{
		m_DialogController.DialogContent.AddChild(content);
		Controller controller;
		content.GetScript(controller);		
		return controller;
	}
	
	protected Controller AddContent(string layout)
	{
		Widget content = GetGame().GetWorkspace().CreateWidgets(layout);
		m_DialogController.DialogContent.AddChild(content);
		Controller controller;
		content.GetScript(controller);		
		return controller;
	}
	
	protected Controller AddContent(EditorPrefab prefab)
	{
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
	

	
	void ShowDialog()
	{
		EditorLog.Trace("EditorDialog::ShowDialog");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
		m_LayoutRoot.Show(true);
		
		if (m_EditorHud.GetModal()) {
			m_EditorHud.GetModal().CloseDialog();
		}
		
		m_Editor.GetCamera().SetMoveEnabled(false);
		m_Editor.GetCamera().SetLookEnabled(false);
		m_EditorHud.ShowCursor();
		
		m_EditorHud.SetModal(this);
		
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
		m_LayoutRoot.Unlink();
		
		m_EditorHud.SetModal(null);
		m_Editor.GetCamera().SetMoveEnabled(true);
		m_Editor.GetCamera().SetLookEnabled(true);
		m_EditorHud.ShowCursor();
	}
	
	void Update();

	
}
