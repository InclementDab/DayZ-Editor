class EditableTextWidgetBetter: ScriptedWidgetEventHandler
{
	
	private int m_CarriagePosition;
	
	private TextWidget m_TextWidget;
	private EditBoxWidget m_EditWidget;
	
	reference string Text;
	private string m_Text;
	
	private ref Timer m_Timer;
	
	void EditableTextWidgetBetter()
	{
		Print("EditableTextWidget");
		m_Timer = new Timer();
		
	}
	
	void ~EditableTextWidgetBetter()
	{
		m_Timer.Stop();
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		Print("EditableTextWidget::OnWidgetScriptInit");
		if (!Class.CastTo(m_TextWidget, w)) {
			EditorLog.Error(string.Format("EditableTextWidget must be used on type TextWidget! Found: %1", w.GetTypeName()));
			return;
		}
		m_Text = Text;
		
		m_TextWidget = w;
		m_EditWidget = EditBoxWidget.Cast(m_TextWidget.FindAnyWidget("EditorListItemTextEditor"));
		
		m_TextWidget.SetHandler(this);
		
		
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		return false;
	}
	

	
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		Print("EditableTextWidget::OnDoubleClick");
		StartEdit();
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
		SetFocus(m_TextWidget);	
		m_CarriagePosition = m_Text.Length();		
		m_Timer.Run(0.01, this, "UpdateEdit", null, true);
	}
	
	private void EndEdit()
	{
		Print("EditableTextWidget::EndEdit");
		m_Timer.Stop();
		m_TextWidget.SetText(m_Text);
	}
	
	private void UpdateEdit()
	{
		
		m_Text = m_EditWidget.GetText();
		
				
		string beg = m_Text.Substring(0, m_CarriagePosition);
		string end = m_Text.Substring(m_CarriagePosition - m_Text.Length(), m_Text.Length());
		
		m_TextWidget.SetText(beg + "|" + end);
	}
	

	
	override bool OnKeyPress(Widget w, int x, int y, int key)
	{
		Print("EditableTextWidget::OnKeyPress");
		
		switch (key) {
			
			case KeyCode.KC_LEFT: {
				m_CarriagePosition -= 1;				
				break;
			}
			
			case KeyCode.KC_RIGHT: {
				m_CarriagePosition += 1;
				break;
			}
			
			case KeyCode.KC_HOME: {
				m_CarriagePosition = 0;
				break;
			}
			
			case KeyCode.KC_END: {
				m_CarriagePosition = m_Text.Length();
				break;
			}
			
			case KeyCode.KC_DELETE: {
				m_CarriagePosition -= 1;
				break;
			}
			
			default: {
				m_CarriagePosition += 1;
				break;
			}
		}
		
		Math.Clamp(m_CarriagePosition, 0, m_Text.Length());
		
		
		return super.OnKeyPress(w, x, y, key);
	}
	
	
}