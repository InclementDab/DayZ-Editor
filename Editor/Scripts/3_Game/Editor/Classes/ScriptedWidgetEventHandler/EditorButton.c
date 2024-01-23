[RegisterCommand(BoxSelectCommand)]
class BoxSelectCommand: Command
{
	override void Execute(bool state)
	{
		super.Execute(state);
		
		if (state) {
			GetDayZGame().GetCommand(CircleSelectCommand).Execute(false);
			GetDayZGame().GetCommand(LassoSelectCommand).Execute(false);
		}
	}
	
	override string GetName()
	{
		return "Box Selection";
	}
}

[RegisterCommand(CircleSelectCommand)]
class CircleSelectCommand: Command
{
	override void Execute(bool state)
	{
		super.Execute(state);
		
		if (state) {
			GetDayZGame().GetCommand(BoxSelectCommand).Execute(false);
			GetDayZGame().GetCommand(LassoSelectCommand).Execute(false);
		}
	}
	
	override string GetName()
	{
		return "Circle Selection";
	}
}

[RegisterCommand(LassoSelectCommand)]
class LassoSelectCommand: Command
{
	override void Execute(bool state)
	{
		super.Execute(state);
		
		if (state) {
			GetDayZGame().GetCommand(BoxSelectCommand).Execute(false);
			GetDayZGame().GetCommand(CircleSelectCommand).Execute(false);
		}
	}
	
	override string GetName()
	{
		return "Lasso Selection";
	}
}

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
			m_Icon.LoadImageFile(0, Icon.WithSize(SymbolSize.REGULAR));
			m_Icon.SetImage(0);
		}
		
		if (CommandType != string.Empty) {
			m_Command = GetDayZGame().GetCommand(CommandType.ToType());
			if (m_Command) {
				m_Command.OnExecute.Insert(OnExecuted);
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
		
		m_Icon.LoadImageFile(0, Icon.WithSize(size));
		m_Icon.SetImage(0);
	}
	
	Symbol GetIcon()
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
#ifndef WORKBENCH
		//WidgetAnimator.Animate(m_Icon, WidgetAnimatorProperty.COLOR_A, 1.0, 50);
#endif
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{		
#ifndef WORKBENCH
		//WidgetAnimator.Animate(m_Icon, WidgetAnimatorProperty.COLOR_A, 100.0 / 255.0, 50);
		
		if (!m_Button.GetState()) {
			//WidgetAnimator.AnimateColor(m_Icon, ARGB(100, 255, 255, 255), 50);
		}
#endif
		
		return true;
	}
}