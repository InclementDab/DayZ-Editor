class EditorDialogCategoryBase: DialogCategoryBase
{
	protected Editor m_Editor;
	protected EditorHud m_EditorHud;
	
	void EditorDialogCategoryBase(string title)
	{
		m_Editor = GetEditor();
		if (m_Editor) {
			m_EditorHud = m_Editor.GetEditorHud();	
		}
		
		if (m_Editor && m_Editor.GeneralSettings.LockCameraDuringDialogs) {
			m_Editor.GetCamera().MoveEnabled = false;
			m_Editor.GetCamera().LookEnabled = false;
			m_EditorHud.ShowCursor(true);
		}
		
		if (EditorHud.CurrentDialog) {
			EditorHud.CurrentDialog.CloseDialog();
		}
		
		EditorHud.CurrentDialog = this;
		
		//! Set Dialog to last saved position
		if (m_EditorHud.GetLastDialogPosition(this)) {
			vector pos = m_EditorHud.GetLastDialogPosition(this);
			m_LayoutRoot.SetPos(pos[0], pos[1]);
			return;
		}
		
		float du, dv, dx, dy;
		m_LayoutRoot.GetScreenSize(du, dv);		
		m_LayoutRoot.GetPos(dx, dy);
		m_LayoutRoot.SetPos(dx, dy - dv / 2);
	}
	
	void ~EditorDialogCategoryBase()
	{
		//! Save last Dialog position
		if (m_LayoutRoot) {
			m_EditorHud.RegisterLastDialogPosition(this);
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
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/dialogs/DialogCategoryBase.layout";
	}
}