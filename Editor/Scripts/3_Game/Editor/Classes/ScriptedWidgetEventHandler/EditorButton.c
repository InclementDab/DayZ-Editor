class EditorButton: ScriptedWidgetEventHandler
{	
	protected Command m_Command;
	
	reference int Red = 255;
	reference int Green = 255;
	reference int Blue = 255;
	reference string CommandType;	
	reference Symbols Icon;
	
	protected ButtonWidget m_Button;
	protected ImageWidget m_Icon;
	
	void OnWidgetScriptInit(Widget w)
	{
		w.SetHandler(this);
		
		if (CommandType != string.Empty && !CommandType.ToType()) {
			Error("Type not found! " + CommandType);
		}
		
		if (CommandType.ToType()) {
			m_Command = GetDayZGame().GetEditor().GetCommand(CommandType.ToType());
		}
		
		m_Button = FindWidget<ButtonWidget>.SearchDown(w, "Button");
		m_Icon = FindWidget<ImageWidget>.SearchDown(w, "Icon");
		if (Icon != string.Empty) {
			m_Icon.LoadImageFile(0, Icon.Regular());
			m_Icon.SetImage(0);
		}
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button == 0 && m_Command) {
			m_Command.Execute(m_Button.GetState());
		}
		
		return true;
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (finished) {
			Print(m_Button); // colors?
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
		if (!m_Button) {
			Print(w.GetName());
			return true;
		}
		
		if (icon && !m_Button.GetState()) {
			WidgetAnimator.AnimateColor(icon, ARGB(100, 255, 255, 255), 100);
		}
		
		return true;
	}
}