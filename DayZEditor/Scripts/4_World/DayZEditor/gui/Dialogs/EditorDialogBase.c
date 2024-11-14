class EditorDialogBase: DialogBase
{
	protected Editor m_Editor;
	protected EditorHud m_EditorHud;
	protected ref ScriptCaller m_SynchronousCallback;
	protected bool m_IsSynchronous;
	
	Widget DialogContent, TitleBar;
	ScrollWidget DialogWrapper;
	
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
		
		if (TitleBar) {
			TitleBar.SetColor(m_Editor.GetSettings().SelectionColor);
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
				DialogWrapper.SetScreenSize(dcu, Math.Max(150, dcv + 1));
			} else {
				DialogWrapper.SetScreenSize(dcu, Math.Max(150, dcv + 1));
			}
			
			float du, dv, dx, dy;
			m_LayoutRoot.GetScreenSize(du, dv);
			m_LayoutRoot.SetScreenPos(sx / 2 - du / 2, sy / 2 - dcv / 2 - 66);
		}
		
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
	
	void ShowDialogSynchronous(ScriptCaller dialog_cb = null)
	{
		m_SynchronousCallback = dialog_cb;
		m_IsSynchronous = true;
		m_LayoutRoot.Show(true);
	}

	override void CloseDialog(DialogResult dialog_result = DialogResult.Cancel)
	{
		if (m_SynchronousCallback) {
			m_SynchronousCallback.Invoke(dialog_result);
		}
		
		Trace("CloseDialog");
		m_DialogResult = dialog_result;
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Delete, 15);
	}
	
	override DialogButton AddButton(DialogButton button)
	{
		DialogButton btn = super.AddButton(button);
		
		btn.Button.SetColor(m_Editor.GetSettings().HighlightColor);
		return btn;
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