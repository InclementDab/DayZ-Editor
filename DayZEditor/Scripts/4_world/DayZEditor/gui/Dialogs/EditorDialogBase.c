class EditorDialogController: Controller
{
	string Title;
	string Caption;
	
	ref ObservableCollection<ref ScriptView> DialogContentData = new ObservableCollection<ref ScriptView>("DialogContentData", this);
	ref ObservableCollection<ref ScriptView> DialogButtonData = new ObservableCollection<ref ScriptView>("DialogButtonData", this);
	
	void ~EditorDialogController()
	{
		delete DialogContentData;
		delete DialogButtonData;
	}
}

class EditorDialogBase: EditorScriptView
{
	// Private members
	private DialogResult m_DialogResult = DialogResult.None;
	
	
	protected EditorDialogController m_EditorDialogController;
	
	void EditorDialogBase(Widget parent = null)
	{
		EditorLog.Trace("EditorDialog");

		m_EditorDialogController = EditorDialogController.Cast(m_Controller);

		m_Editor.GetCamera().MoveEnabled = false;
		m_Editor.GetCamera().LookEnabled = false;
		m_EditorHud.ShowCursor(true);
		
		EditorUIManager.CurrentDialog = this;
		
		float du, dv, dx, dy;
		m_LayoutRoot.GetScreenSize(du, dv);		
		m_LayoutRoot.GetPos(dx, dy);
		m_LayoutRoot.SetPos(dx, dy - dv / 2);
	}
	
	private void ~EditorDialogBase()
	{
		EditorLog.Trace("~EditorDialog");
		m_Editor.GetCamera().MoveEnabled = true;
		m_Editor.GetCamera().LookEnabled = true;
		m_EditorHud.ShowCursor(true);
	}
	
	DialogResult ShowDialog()
	{
		EditorLog.Trace("EditorDialog::ShowDialog");
		m_LayoutRoot.Show(true);
		while (m_DialogResult == DialogResult.None) {
			Sleep(1);
		}
		
		EditorLog.Debug("DialogResult: %1", typename.EnumToString(DialogResult, m_DialogResult));
		return m_DialogResult;
	}
	
	void CloseDialog()
	{
		EditorLog.Trace("EditorDialog::CloseDialog");
		m_DialogResult = DialogResult.Cancel;
		delete this;
	}
	
	void CloseDialog(DialogResult dialog_result)
	{
		EditorLog.Trace("EditorDialog::CloseDialog");
		m_DialogResult = dialog_result;
		delete this;
	}
	
	void AddContent(ScriptView content)
	{
		m_EditorDialogController.DialogContentData.Insert(content);
	}
	
	void AddButton(ScriptView button)
	{
		m_EditorDialogController.DialogButtonData.Insert(button);
	}
		
	override typename GetControllerType() {
		return EditorDialogController;
	}	
		
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/EditorDialog.layout";
	}
}