class EditorCommandButton: EditorButton
{
	protected CommandNode m_CommandNode;
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		m_CommandNode = CommandNode.Cast(m_Node);
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_Node) {
			return false;
		}
		
		switch (m_CommandNode.GetShortcutType()) {
			case ShortcutKeyType.PRESS:
			case ShortcutKeyType.HOLD: {
				GetDayZGame().GetEditor().Select(m_CommandNode);
				return true;
			}
		}
				
		return false;
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_CommandNode) {
			return false;
		}
		
		switch (m_CommandNode.GetShortcutType()) {
			case ShortcutKeyType.HOLD: {
				GetDayZGame().GetEditor().Deselect(m_CommandNode);
				return true;
			}
		}
				
		return false;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_CommandNode) {
			return false;
		}
		
		switch (m_CommandNode.GetShortcutType()) {
			case ShortcutKeyType.TOGGLE: {
				GetDayZGame().GetEditor().ToggleSelect(m_CommandNode);
				return true;
			}
		}
				
		return false;
	}
		
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_CommandNode) {
			return false;
		}
		
		switch (m_CommandNode.GetShortcutType()) {
			case ShortcutKeyType.DOUBLE: {
				GetDayZGame().GetEditor().ToggleSelect(m_CommandNode);
				return true;
			}
		}
		
		return false;
	}
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_CommandNode) {
			GetDayZGame().GetEditor().GetHud().SetCursor(m_Icon, m_CommandNode.GetDisplayName(), m_CommandNode.GetShortcutString());
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{				
		if (m_Node && !m_Node.GetEditor().IsSelected(m_Node)) {
			WidgetAnimator.Animate(m_IconWidget, WidgetAnimatorProperty.COLOR_A, 100.0 / 255.0, 50);
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
}