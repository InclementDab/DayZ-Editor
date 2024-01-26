class EditorButton: ScriptedWidgetEventHandler
{	
	protected Command m_Command;
	
	reference string CommandType;	
	reference Symbols Icon;
	
	protected Widget m_LayoutRoot;
	protected ButtonWidget m_Button;
	protected ImageWidget m_Icon;
	
	void OnWidgetScriptInit(Widget w)
	{
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
	
		m_Button = FindWidget<ButtonWidget>.SearchDown(m_LayoutRoot, "Button");
		m_Button.SetHandler(this);
		
		m_Icon = FindWidget<ImageWidget>.SearchDown(m_LayoutRoot, "Icon");
		
		if (Icon != string.Empty) {
			m_Icon.LoadImageFile(0, Icon.Regular());
			m_Icon.SetImage(0);
		}
		
		if (CommandType != string.Empty) {
			m_Command = GetDayZGame().GetCommand(CommandType.ToType());
			if (m_Command) {
				m_Command.OnExecute.Insert(OnExecuted);				
				m_Button.SetState(m_Command.GetDefaultState());
			}
		}
		
#ifdef WORKBENCH
		// debug display
		//m_Icon.SetColor(m_LayoutRoot.GetColor());
#endif
	}
			
	void OnExecuted(bool state)
	{
		SymbolSize size = Ternary<SymbolSize>.If(state, SymbolSize.SOLID, SymbolSize.REGULAR);
		int color = Ternary<int>.If(state, m_LayoutRoot.GetColor(),	ARGB(100, 255, 255, 255));
		
		WidgetAnimator.AnimateColor(m_Icon, color, 50);
		
		m_Icon.LoadImageFile(0, Ternary<string>.If(state, Icon.Solid(), Icon.Regular()));
		m_Icon.SetImage(0);
	}
	
	Symbols GetIcon()
	{
		return Icon;
	}
	
	Widget GetLayoutRoot()
	{
		return m_LayoutRoot;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button == 0 && m_Command) {
			m_Command.Execute(!m_Command.IsExecuted());
		}
		
		return true;
	}
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		WidgetAnimator.Animate(m_Icon, WidgetAnimatorProperty.COLOR_A, 1.0, 50);
		
		if (m_Command) {
			GetDayZGame().GetEditor().GetHud().SetCursor(Icon, m_Command.GetName(), m_Command.GetShortcutString());
		}
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{				
		GetDayZGame().GetEditor().GetHud().ClearCursor();
		
		if (!m_Button.GetState()) {
			WidgetAnimator.Animate(m_Icon, WidgetAnimatorProperty.COLOR_A, 100.0 / 255.0, 50);
		}
		
		return true;
	}
}