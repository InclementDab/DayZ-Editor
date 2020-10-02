
class EditorMessageBox: MessageBox
{
	protected Editor m_Editor;
	protected EditorHud m_EditorHud;
	
	void EditorMessageBox(Widget parent = null, string title = "", string caption = "", MessageBoxButtons buttons = MessageBoxButtons.OK)
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
		
		float du, dv, dx, dy;
		m_LayoutRoot.GetScreenSize(du, dv);		
		m_LayoutRoot.GetPos(dx, dy);
		m_LayoutRoot.SetPos(dx, dy - dv / 2);
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
		EditorMessageBox message_box = new EditorMessageBox(null, title, caption, buttons);
		return message_box.ShowDialog();
	}
}