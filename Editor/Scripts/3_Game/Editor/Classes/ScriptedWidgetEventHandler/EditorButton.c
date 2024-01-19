class EditorButton: ScriptedWidgetEventHandler
{	
	protected Command m_Command;
	
	reference int Red = 255;
	reference int Green = 255;
	reference int Blue = 255;
	reference string CommandType;	
	
	void OnWidgetScriptInit(Widget w)
	{
		w.SetHandler(this);
		
		if (CommandType.ToType()) {
			m_Command = GetEditor().GetCommand(CommandType.ToType());
			Print(m_Command);
		}
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		ButtonWidget button_widget = ButtonWidget.Cast(w);
		if (button == 0 && button_widget && m_Command) {
			m_Command.Execute(button_widget.GetState());
		}
		
		return true;
	}
	
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		Widget icon = ScriptView.FindWidgetClass(w, "Icon");
		if (icon) {
			WidgetAnimator.AnimateColor(icon, ARGB(255, Red, Green, Blue), 100);
		}
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		Widget icon = ScriptView.FindWidgetClass(w, "Icon");
		if (icon) {
			WidgetAnimator.AnimateColor(icon, ARGB(100, 255, 255, 255), 100);
		}
		
		return true;
	}
}