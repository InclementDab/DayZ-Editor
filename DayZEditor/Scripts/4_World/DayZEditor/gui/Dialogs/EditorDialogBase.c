class EditorDialogBase: DialogBase
{
	protected Editor m_Editor;
	protected EditorHud m_EditorHud;
	
	void EditorDialogBase(string title)
	{
		m_Editor = GetEditor();
		if (m_Editor) {
			m_EditorHud = m_Editor.GetEditorHud();	
		}
		
		if (m_Editor && m_Editor.Settings.LockCameraDuringDialogs) {
			m_Editor.GetCamera().MoveEnabled = false;
			m_Editor.GetCamera().LookEnabled = false;
			m_EditorHud.ShowCursor(true);
		}
		
		if (EditorHud.CurrentDialog) {
			EditorHud.CurrentDialog.CloseDialog();
		}
		
		EditorHud.CurrentDialog = this;
		
		//! Set Dialog to last saved position
		if (EditorHud.DialogLastX != -1 && EditorHud.DialogLastY != -1) {
			//! disabled due to issues
			//m_LayoutRoot.SetPos(EditorHud.DialogLastX, EditorHud.DialogLastY);
			//return;
		}
		
		float du, dv, dx, dy;
		m_LayoutRoot.GetScreenSize(du, dv);		
		m_LayoutRoot.GetPos(dx, dy);
		m_LayoutRoot.SetPos(dx, dy - dv / 2);
	}
	
	void ~EditorDialogBase()
	{
		//! Save last Dialog position
		float dx, dy;
		if (m_LayoutRoot) {
			m_LayoutRoot.GetPos(dx, dy);
			EditorHud.DialogLastX = dx;
			EditorHud.DialogLastY = dy;
		}
		
		if (m_Editor) {
			m_Editor.GetCamera().MoveEnabled = true;
			m_Editor.GetCamera().LookEnabled = true;
			if (m_EditorHud) {
				m_EditorHud.ShowCursor(true);
			}
		}
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		return true; // Dialogs should consume the event whenever you click within
	}
}