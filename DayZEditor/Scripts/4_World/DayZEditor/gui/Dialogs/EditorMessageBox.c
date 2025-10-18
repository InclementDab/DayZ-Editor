class EditorMessageBox_DEPRECATED: MessageBox
{
	protected Editor m_Editor;
	protected EditorHud m_EditorHud;
	
	Widget DialogContent, TitleBar;
	ScrollWidget DialogWrapper;
	
	void EditorMessageBox_DEPRECATED(string title, string caption, MessageBoxButtons buttons)
	{
		//EditorLog.Trace("EditorMessageBox_DEPRECATED");

		m_Editor = GetEditor();
		if (!m_Editor) {
			return;
		}
		
		m_EditorHud = m_Editor.GetEditorHud();			
		m_EditorHud.ShowCursor(true);
		
		if (EditorHud.CurrentDialog) {
			EditorHud.CurrentDialog.CloseDialog();
		}
		
		EditorHud.CurrentDialog = this;
		m_EditorHud.ClearCurrentTooltip();
		
		Show(false);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetupDialog, 0, false);
	}
	
	void SetupDialog()
	{
		//! Set Dialog to last saved position
		if (m_EditorHud && m_EditorHud.GetLastDialogPosition(this)) {
			vector pos = m_EditorHud.GetLastDialogPosition(this);
			m_LayoutRoot.SetPos(pos[0], pos[1]);
		} else {
			int sx, sy;
			GetScreenSize(sx, sy);
			float dcu, dcv;	
			DialogContent.GetScreenSize(dcu, dcv);
			if (dcv > sy * 0.8) {
				dcv = sy * 0.8;
				DialogWrapper.SetScreenSize(dcu, Math.Max(25, dcv + 1));
			} else {
				DialogWrapper.SetScreenSize(dcu, Math.Max(25, dcv + 1));
			}
			
			float du, dv, dx, dy;
			m_LayoutRoot.GetScreenSize(du, dv);
			m_LayoutRoot.SetScreenPos(sx / 2 - du / 2, sy / 2 - dcv / 2 - 66);
		}
		
		Show(true);
	}
	
	private void ~EditorMessageBox_DEPRECATED()
	{
		//EditorLog.Trace("~EditorMessageBox_DEPRECATED");
		
		if (!m_Editor) {
			return;
		}
		
		m_EditorHud.ShowCursor(true);
	}
	
	override DialogButton AddButton(DialogButton button)
	{
		DialogButton btn = super.AddButton(button);
		btn.Button.SetColor(GetEditor().GetSettings().HighlightColor);
		return btn;
	}
	
	override static DialogResult Show(string title, string caption, MessageBoxButtons buttons)
	{
		EditorMessageBox_DEPRECATED message_box = new EditorMessageBox_DEPRECATED(title, caption, buttons);
		return message_box.ShowDialog();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/dialogs/DialogBase.layout";
	}
}