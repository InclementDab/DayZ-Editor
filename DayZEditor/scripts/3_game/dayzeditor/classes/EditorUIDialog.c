


class EditorUIDialog: UIScriptedMenu
{
	
	protected ButtonWidget m_DialogBtnConfirm;
	protected ButtonWidget m_DialogBtnCancle;
	
	protected ButtonWidget m_TitleClose;
	protected RichTextWidget m_DialogText;
	
	protected TextWidget m_TitleText;
	
	
	ref ScriptInvoker m_OnDialogConfirm;
	ref ScriptInvoker m_OnDialogCancle;
	
	protected UIScriptedMenu m_Parent;
	
	void EditorUIDialog(UIScriptedMenu parent)
	{
		m_Parent = parent;
	}
	
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/layouts/dialogs/EditorQuestionDialog.layout", null);
		layoutRoot.Show(true);
		
		SetDialog();
		
		return layoutRoot;
	}
	
	
	private void SetDialog()
	{
		m_DialogBtnConfirm = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ConfirmtButton"));
		m_DialogBtnCancle = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CancleButton"));
		m_DialogText = RichTextWidget.Cast(layoutRoot.FindAnyWidget("QuestionText"));
		m_TitleText = TextWidget.Cast(layoutRoot.FindAnyWidget("TitleText"));
		m_TitleClose = ButtonWidget.Cast(layoutRoot.FindAnyWidget("TitleClose"));
	}
	
	void SetTitleText(string text)
	{
		m_TitleText.SetText(text);
	}
	
	void SetDialogText(string text)
	{
		m_DialogText.SetText(text);
	}
	
	UIScriptedMenu ShowDialog()
	{
		UIScriptedMenu m = GetGame().GetUIManager().ShowScriptedMenu(this, m_Parent);
		SetModal(layoutRoot);
		return m;
	}
	
	override void OnShow()
	{
		EditorPrint("EditorDialogBase::OnShow");
		//GetGame().GetUIManager().ShowCursor(true);
	}
	
	override void OnHide()
	{
		EditorPrint("EditorDialogBase::OnHide");
		//GetGame().GetUIManager().ShowCursor(true);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button == 0) {
			switch (w) {
				case m_TitleClose: {
					Close();
					return true;
				}
				
			}
		}
		return false;
	}
}