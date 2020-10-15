
class EditorMessageBox: MessageBox
{
	protected Editor m_Editor;
	protected EditorHud m_EditorHud;
	
	void EditorMessageBox(string title, string caption, MessageBoxButtons buttons)
	{
		EditorLog.Trace("EditorMessageBox");

		m_Editor = GetEditor();
		if (m_Editor) {
			m_EditorHud = m_Editor.GetEditorHud();	
		}
		m_Editor.GetCamera().MoveEnabled = false;
		m_Editor.GetCamera().LookEnabled = false;
		m_EditorHud.ShowCursor(true);
		
		if (EditorUIManager.CurrentDialog) {
			EditorUIManager.CurrentDialog.CloseDialog();
		}
		
		EditorUIManager.CurrentDialog = this;
	}
	
	private void ~EditorMessageBox()
	{
		EditorLog.Trace("~EditorMessageBox");
		m_Editor.GetCamera().MoveEnabled = true;
		m_Editor.GetCamera().LookEnabled = true;
		m_EditorHud.ShowCursor(true);
	}
	
	override static DialogResult Show(string title, string caption, MessageBoxButtons buttons)
	{
		EditorMessageBox message_box = new EditorMessageBox(title, caption, buttons);
		return message_box.ShowDialog();
	}
}