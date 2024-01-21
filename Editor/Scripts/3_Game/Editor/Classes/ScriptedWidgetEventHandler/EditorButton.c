class EditorButton: ScriptedWidgetEventHandler
{	
	protected Command m_Command;
	
	reference int Red = 255;
	reference int Green = 255;
	reference int Blue = 255;
	reference string CommandType;	
	reference string ButtonName;
	
	protected ButtonWidget m_ButtonWidget;
	
	void OnWidgetScriptInit(Widget w)
	{
		w.SetHandler(this);
		
		if (CommandType != string.Empty && !CommandType.ToType()) {
			Error("Type not found! " + CommandType);
		}
		
		if (CommandType.ToType()) {
			
			m_Command = GetDayZGame().GetEditor().GetCommand(CommandType.ToType());
		}
		
		m_ButtonWidget = ButtonWidget.Cast(w.FindAnyWidget(ButtonName));
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button == 0) {
			m_Command.Execute(m_ButtonWidget.GetState());
		}
		
		return true;
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (finished) {
			Print(m_ButtonWidget); // colors?
		}
		
		return super.OnChange(w, x, y, finished);
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
		if (!m_ButtonWidget) {
			Print(w.GetName());
			return true;
		}
		
		if (icon && !m_ButtonWidget.GetState()) {
			WidgetAnimator.AnimateColor(icon, ARGB(100, 255, 255, 255), 100);
		}
		
		return true;
	}
}