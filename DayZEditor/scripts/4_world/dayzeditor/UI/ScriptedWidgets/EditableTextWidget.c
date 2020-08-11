class EditableTextWidget: ScriptedWidgetEventHandler
{
	
	private TextWidget m_TextWidget;
	private EditBoxWidget m_EditWidget;
	
	reference string Text;
	private string m_Text;
	
	private ref Timer m_Timer;
	
	void EditableTextWidget()
	{
		Print("EditableTextWidget");
		m_Timer = new Timer();
		
	}
	
	void ~EditableTextWidget()
	{
		m_Timer.Stop();
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		Print("EditableTextWidget::OnWidgetScriptInit");
		if (!Class.CastTo(m_TextWidget, w)) {
			EditorPrint(string.Format("EditableTextWidget must be used on type TextWidget! Found: %1", w.GetTypeName()), LogSeverity.ERROR);
			return;
		}
		m_Text = Text;
		m_TextWidget = w;
		m_EditWidget = EditBoxWidget.Cast(m_TextWidget.FindAnyWidget("EditorListItemTextEditor"));
		
		m_TextWidget.SetHandler(this);
		
		//m_Timer.Run(0.01, this, "Update", null, true);
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("OnClick");
		return false;
	}
	
	bool OnChange(Widget w, int x, int y, bool finished)
	{
		m_TextWidget.SetText(m_EditWidget.GetText());
		return true;
	}
	
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		Print("EditableTextWidget::OnDoubleClick");
		//m_TextWidget.SetText(Text + "|");
		SetFocus(m_EditWidget);
		
		//SetModal(m_EditWidget);
		
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
	/*
	override bool OnFocus(Widget w, int x, int y)
	{
		Print("EditableTextWidget::OnFocus");
		return true;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		Print("EditableTextWidget::OnFocusLost");
		return true;
	}
	

	
	override bool OnKeyPress(Widget w, int x, int y, int key)
	{
		Print("EditableTextWidget::OnKeyPress");
		
		
		
		//m_Text += key.AsciiToString();
		
		return true;
	}
	
	override bool OnKeyDown(Widget w, int x, int y, int key)
	{
		Print("EditableTextWidget::OnKeyDown");
		
		return true;
	}*/
	
}