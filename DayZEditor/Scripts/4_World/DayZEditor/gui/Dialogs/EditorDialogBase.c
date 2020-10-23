class EditorDialogBase: DialogBase
{
	protected Editor m_Editor;
	protected EditorHud m_EditorHud;
	
	void EditorDialogBase(string title)
	{
		m_Editor = GetEditor();
		if (m_Editor && m_Editor.Settings.LockCameraDuringDialogs) {
			m_EditorHud = m_Editor.GetEditorHud();	
			m_Editor.GetCamera().MoveEnabled = false;
			m_Editor.GetCamera().LookEnabled = false;
			m_EditorHud.ShowCursor(true);
		}
		
		if (EditorUIManager.CurrentDialog) {
			EditorUIManager.CurrentDialog.CloseDialog();
		}
		
		EditorUIManager.CurrentDialog = this;
		
		//! Set Dialog to last saved position
		if (EditorUIManager.DialogLastX != -1 && EditorUIManager.DialogLastY != -1) {
			m_LayoutRoot.SetPos(EditorUIManager.DialogLastX, EditorUIManager.DialogLastY);
			return;
		}
		
		float du, dv, dx, dy;
		m_LayoutRoot.GetScreenSize(du, dv);		
		m_LayoutRoot.GetPos(dx, dy);
		m_LayoutRoot.SetPos(dx, dy - dv / 2);
	}
	
	private void ~EditorDialogBase()
	{
		//! Save last Dialog position
		float dx, dy;
		m_LayoutRoot.GetPos(dx, dy);
		EditorUIManager.DialogLastX = dx;
		EditorUIManager.DialogLastY = dy;
		
		if (m_Editor) {
			m_Editor.GetCamera().MoveEnabled = true;
			m_Editor.GetCamera().LookEnabled = true;
			m_EditorHud.ShowCursor(true);
		}
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		return true; // Dialogs should consume the event whenever you click within
	}
}
