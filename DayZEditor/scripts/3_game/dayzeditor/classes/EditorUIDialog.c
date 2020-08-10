enum EditorDialogType
{
	QUESTION = 0
}

class EditorUIDialog extends ScriptedWidgetEventHandler
{
	private Widget m_Root;
	
	private ButtonWidget m_DialogBtnConfirm;
	private ButtonWidget m_DialogBtnCancle;
	private RichTextWidget m_DialogText;
	
	private EditorDialogType m_Type;
	
	ref ScriptInvoker m_OnDialogConfirm;
	ref ScriptInvoker m_OnDialogCancle;
	
	void EditorUIDialog(Widget parent, EditorDialogType type)
	{
		m_Root = GetGame().GetWorkspace().CreateWidgets(GetLayout(type), parent);
		
		m_Type = type;
		
		SetDialog();
	}
	
	string GetLayout(EditorDialogType type)
	{
		string layout;
		switch (type)
		{
			case EditorDialogType.QUESTION:
			{
				layout = "DayZEditor/gui/layouts/dialogs/EditorQuestionDialog.layout";
			}
			break;
		}
		
		return layout;
	}
	
	private void SetDialog()
	{
		switch (m_Type)
		{
			case EditorDialogType.QUESTION:
			{
				m_DialogBtnConfirm = ButtonWidget.Cast(m_Root.FindAnyWidget("ConfirmtButton"));
				m_DialogBtnCancle = ButtonWidget.Cast(m_Root.FindAnyWidget("CancleButton"));
				m_DialogText = RichTextWidget.Cast(m_Root.FindAnyWidget("QuestionText"));
				
				m_OnDialogConfirm = new ScriptInvoker();
				m_OnDialogCancle = new ScriptInvoker();
			}
			break;
		}
	}
	
	void SetText(string text)
	{
		if (m_Type == EditorDialogType.QUESTION)
		{
			m_DialogText.SetText(text);
		}
	}	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (m_Type == EditorDialogType.QUESTION)
		{
			if(w == m_DialogBtnConfirm)
			{
				m_OnDialogConfirm.Invoke();
			}else if(w == m_DialogBtnCancle)
			{
				m_OnDialogCancle.Invoke();
			}
		}
		return false;
	}
}