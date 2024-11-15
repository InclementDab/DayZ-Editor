class EditorButtonScript: ScriptedWidgetEventHandler
{
	reference string CommandType;

	protected Widget m_LayoutRoot;
	protected LinearColor m_DefaultColor, m_DefaultIconColor;
	protected EditorCommand m_Command;
	protected ImageWidget Icon;

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
			Icon.SetImage(1);
			m_DefaultIconColor = Icon.GetColor();
		}
				
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
#endif
	}

	protected void Update(float dt)
	{
		if (m_Command && Icon) {
	
			if (m_Command.IsToggled()) {
				Icon.SetColor(m_Command.GetColor());
			} else {
				Icon.SetColor(LinearColor.WHITE);
			}

			if (!m_Command.CanExecute()) {
				Icon.SetAlpha(0.3);
			} else {
				Icon.SetAlpha(1.0);
			}
		}
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (m_Command) {
			m_Command.Execute(this, CommandArgs());
			
			if (m_Command.IsToggled()) {
				Icon.SetImage(3);
			} else {
				Icon.SetImage(2);
			}
		}

		return true;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_Command && !EditorHud.CurrentMenu) {
			float pos_x, pos_y, size_x, size_y;
			m_LayoutRoot.GetScreenPos(pos_x, pos_y);
			m_LayoutRoot.GetScreenSize(size_x, size_y);
							
			EditorTooltip tooltip = EditorTooltip.CreateOnButton(m_Command, w, TooltipPositions.BOTTOM_LEFT);
			if (!m_Command.CanExecute()) {
				tooltip.GetLayoutRoot().SetAlpha(100);
			}
			
			if (!m_Command.IsToggled()) {
				Icon.SetImage(2);
			}
			
			WidgetAnimator.AnimateColor(m_LayoutRoot, GetEditor().GetSettings().HighlightColor, 70);
				
			GetEditor().GetEditorHud().DelaySetCurrentTooltip(tooltip, w);
		}

		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_Command) {
			GetEditor().GetEditorHud().SetCurrentTooltip(null);
			
			if (!m_Command.IsToggled()) {
				Icon.SetImage(1);
			}
			
			WidgetAnimator.AnimateColor(m_LayoutRoot, m_DefaultColor, 35);
		}
		
		return true;
	}

	protected void DelayedTooltipCheck(Widget w)
	{
		if (w != GetWidgetUnderCursor()) {
			return;
		}

	}
}