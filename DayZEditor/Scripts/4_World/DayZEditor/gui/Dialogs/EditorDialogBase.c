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
		
		if (m_EditorHud) {
			m_EditorHud.ShowCursor(true);
		}
		
		if (EditorHud.CurrentDialog) {
			EditorHud.CurrentDialog.CloseDialog();
		}
		
		EditorHud.CurrentDialog = this;
		m_EditorHud.ClearCurrentTooltip();
		
		//! Set Dialog to last saved position
		if (m_EditorHud && m_EditorHud.GetLastDialogPosition(this)) {
			//vector pos = m_EditorHud.GetLastDialogPosition(this);
			//m_LayoutRoot.SetPos(pos[0], pos[1]);
			//return;
		}
		
		Show(false);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetupDialog, 3, false);
	}
	
	void SetupDialog()
	{
		float du, dv, dx, dy;
		int sx, sy;
		GetScreenSize(sx, sy);
		m_LayoutRoot.GetScreenSize(du, dv);
		m_LayoutRoot.SetScreenPos(sx / 2 - du / 2, sy / 2 - dv);
		Show(true);
	}
	
	void ~EditorDialogBase()
	{
		//! Save last Dialog position
		if (m_LayoutRoot && m_EditorHud) {
			m_EditorHud.RegisterLastDialogPosition(this);
		}
		
		if (m_EditorHud) {
			m_EditorHud.ShowCursor(true);
		}
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		return true; // Dialogs should consume the event whenever you click within
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/dialogs/DialogBase.layout";
	}
}