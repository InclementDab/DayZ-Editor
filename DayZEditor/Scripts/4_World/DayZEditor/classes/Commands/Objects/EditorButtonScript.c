class EditorButtonScript: ScriptedWidgetEventHandler
{
	reference string CommandType;
	
	static EditorButtonScript PressedButton;
	static int PressedButtonButton = -1;

	static const int ICON_SIZE_NORMAL = 1;
	static const int ICON_SIZE_HOVER = 2;
	static const int ICON_SIZE_ENABLE = 3;

	protected Widget m_LayoutRoot;
	protected LinearColor m_DefaultColor, m_DefaultIconColor;
	protected EditorCommand m_Command;
	protected ImageWidget Icon;
	protected float m_TickAccumulated;

	void OnWidgetScriptInit(Widget w)
	{
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
		m_DefaultColor = m_LayoutRoot.GetColor();
		
#ifndef COMPONENT_SYSTEM
		typename command_type = CommandType.ToType();		
		m_Command = GetEditor().CommandManager[command_type];
		if (!m_Command) {
			return;
		}

		Icon = ImageWidget.Cast(m_LayoutRoot.GetChildren());
		if (Icon) {
			m_DefaultIconColor = Icon.GetColor();
			Symbols symbol = m_Command.GetSymbol();
			if (symbol) {
				symbol.Load(Icon, ICON_SIZE_NORMAL);
			}
		}
				
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
#endif
	}

	protected void Update(float dt)
	{
		m_TickAccumulated += dt;
		if (m_TickAccumulated < 0.124) {
			return;
		}
		
		if (!Icon) {
			GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
			return;
		}
		
		if (!m_Command) {
			return;
		}
		
		bool hovering = GetWidgetUnderCursor() == m_LayoutRoot;
		bool can_execute = m_Command.CanExecute();
		bool mouse_state = GetUApi().GetInputByID(UAMenuSelect).LocalValue();
		if (m_Command.IsToggled()) {
			if (hovering && mouse_state) {
				Icon.SetImage(1);
			} else {
				Icon.SetImage(ICON_SIZE_ENABLE);
			}
			
			Icon.SetColor(m_Command.GetColor());
			
		} else {
			if (hovering && can_execute) {
				if (mouse_state) {
					Icon.SetImage(3);
				} else {
					Icon.SetImage(1);
				}
				Icon.SetColor(m_Command.GetColor());
			} else {
				Icon.SetImage(2);
				Icon.SetColor(-1);
			}
		}

		if (!can_execute) {
			Icon.SetAlpha(0.3);
		} else {
			Icon.SetAlpha(1.0);
		}
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		PressedButton = this;
		PressedButtonButton = button;
		
		//WidgetAnimator.AnimateColor(Icon, m_Command.GetColor(), 100);
		Icon.SetImage(3);
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (PressedButton != this || PressedButtonButton != button) {
			PressedButton = null;
			PressedButtonButton = -1;
			return false;
		}

		if (m_Command) {
			m_Command.Execute(this, CommandArgs());
			return true;
		}

		return super.OnMouseButtonUp(w, x, y, button);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_Command && !EditorHud.CurrentMenu && m_Command.CanExecute()) {			
			float pos_x, pos_y, size_x, size_y;
			m_LayoutRoot.GetScreenPos(pos_x, pos_y);
			m_LayoutRoot.GetScreenSize(size_x, size_y);
							
			EditorTooltip tooltip = EditorTooltip.CreateOnButton(m_Command, w, TooltipPositions.BOTTOM_LEFT);
			if (!m_Command.CanExecute()) {
				tooltip.GetLayoutRoot().SetAlpha(100);
			} else {
				WidgetAnimator.AnimateColor(m_LayoutRoot, GetEditor().GetSettings().HighlightColor, 70);
			}
				
			g_Game.DelaySetCurrentTooltip(tooltip, w);
		}

		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_Command) {
			GetEditor().GetEditorHud().ClearCurrentTooltip();
						
			WidgetAnimator.AnimateColor(m_LayoutRoot, m_DefaultColor, 35);
		}
		
		return true;
	}
}