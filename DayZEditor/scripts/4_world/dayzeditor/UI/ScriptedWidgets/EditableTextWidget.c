class EditableTextWidget: ScriptedWidgetEventHandler
{
	
	private bool m_IsEditing = false;
	
	private Widget m_EditFrame;
	private TextWidget m_TextWidget;
	private EditBoxWidget m_EditWidget;
	
	private string m_Text;
	reference string Text;
	
	void EditableTextWidget()
	{
		Print("EditableTextWidget");
		
	}
	
	void ~EditableTextWidget()
	{
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		Print("EditableTextWidget::OnWidgetScriptInit");
		
		m_EditFrame = w;
		m_TextWidget = TextWidget.Cast(m_EditFrame.FindAnyWidget("EditorListItemLabel"));
		m_EditWidget = EditBoxWidget.Cast(m_EditFrame.FindAnyWidget("EditorListItemTextEditor"));
		
		m_Text = Text;
		
		m_EditFrame.SetHandler(this);
	}
	
	void SetText(string text)
	{
		Print(text);
		m_Text = text;
		m_TextWidget.SetText(text);
	}
	
	
		
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		Print("EditableTextWidget::OnDoubleClick");
		SetFocus(m_EditFrame);
		return true;
	}
	
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return false;
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		Print("EditableTextWidget::OnFocus");
		StartEdit();
		return true;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		Print("EditableTextWidget::OnFocusLost");
		EndEdit();
		return true;
	}
	
	
	private void StartEdit()
	{
		Print("EditableTextWidget::StartEdit");	
		m_IsEditing = true;
		
		m_EditWidget.SetText(m_Text);
		m_EditWidget.Show(m_IsEditing);
		m_TextWidget.Show(!m_IsEditing);
	}
	
	private void EndEdit()
	{
		Print("EditableTextWidget::EndEdit");
		m_IsEditing = false;
		m_Text = m_EditWidget.GetText();
		m_TextWidget.SetText(m_Text);
		
		m_TextWidget.Show(m_IsEditing);
		m_EditWidget.Show(!m_IsEditing);
	}
	
	

	
	
	
	
	
}