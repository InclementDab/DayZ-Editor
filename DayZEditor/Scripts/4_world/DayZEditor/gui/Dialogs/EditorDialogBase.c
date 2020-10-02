class EditorDialogBase: DialogBase
{
	protected Editor m_Editor;
	protected EditorHud m_EditorHud;
	
	void EditorDialogBase(Widget parent, string title)
	{
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
	
	private void ~EditorDialogBase()
	{
		m_Editor.GetCamera().MoveEnabled = true;
		m_Editor.GetCamera().LookEnabled = true;
		m_EditorHud.ShowCursor(true);
	}
}