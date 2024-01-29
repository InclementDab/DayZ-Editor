class EditorButton: ScriptedWidgetEventHandler
{		
	reference string Node;
	
	protected CommandNode m_Node;
	protected Widget m_LayoutRoot;
	
	protected TextWidget m_TextWidget;
	protected ButtonWidget m_Button;
	protected ImageWidget m_IconWidget;
	
	void OnWidgetScriptInit(Widget w)
	{
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
	
#ifdef WORKBENCH
		if (!GetDayZGame()) {
			return;
		}
#endif
		
		m_Button = FindWidget<ButtonWidget>.SearchDown(m_LayoutRoot, "Button");				
		m_IconWidget = FindWidget<ImageWidget>.SearchDown(m_LayoutRoot, "Icon");		
		m_TextWidget = FindWidget<TextWidget>.SearchDown(m_LayoutRoot, "Text");
		
		if (Node == string.Empty) {
			return;
		}
		
		m_Node = CommandNode.Cast(GetDayZGame().GetEditor().GetNode(Node));
		if (!m_Node) {
			Error(string.Format("Could not find node %1", Node));
			return;
		}

		m_Node.AfterSelectionChanged.Insert(OnExecuted);
		GetDayZGame().GetEditor().Select(m_Node);
		OnExecuted(m_Node, m_Node.GetDefaultState());
	}
	
	void OnExecuted(TreeNode node, bool state)
	{
		SymbolSize size = Ternary<SymbolSize>.If(state, SymbolSize.SOLID, SymbolSize.REGULAR);
		int color = Ternary<int>.If(state, m_LayoutRoot.GetColor(),	ARGB(100, 255, 255, 255));
				
		Symbols icon = m_Node.GetIcon();
		if (m_IconWidget) {
			WidgetAnimator.AnimateColor(m_IconWidget, color, 50);
			m_IconWidget.LoadImageFile(0, Ternary<Symbol>.If(state, icon.Solid(), icon.Regular()));
			m_IconWidget.SetImage(0);
		}
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_Node) {
			return false;
		}
		
		switch (m_Node.GetShortcutType()) {
			case ShortcutKeyType.PRESS:
			case ShortcutKeyType.HOLD: {
				GetDayZGame().GetEditor().Select(m_Node);
				return true;
			}
		}
				
		return false;
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_Node) {
			return false;
		}
		
		switch (m_Node.GetShortcutType()) {
			case ShortcutKeyType.HOLD: {
				GetDayZGame().GetEditor().Deselect(m_Node);
				return true;
			}
		}
				
		return false;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_Node) {
			return false;
		}
		
		switch (m_Node.GetShortcutType()) {
			case ShortcutKeyType.TOGGLE: {
				GetDayZGame().GetEditor().ToggleSelect(m_Node);
				return true;
			}
		}
				
		return false;
	}
		
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_Node) {
			return false;
		}
		
		switch (m_Node.GetShortcutType()) {
			case ShortcutKeyType.DOUBLE: {
				GetDayZGame().GetEditor().ToggleSelect(m_Node);
				return true;
			}
		}
		
		return false;
	}
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		WidgetAnimator.Animate(m_IconWidget, WidgetAnimatorProperty.COLOR_A, 1.0, 50);
		GetDayZGame().GetEditor().GetHud().SetCursor(m_Node.GetIcon(), m_Node.GetDisplayName(), m_Node.GetShortcutString());		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		GetDayZGame().GetEditor().GetHud().ClearCursor();
					
		if (m_Node && !m_Node.GetEditor().IsSelected(m_Node)) {
			WidgetAnimator.Animate(m_IconWidget, WidgetAnimatorProperty.COLOR_A, 100.0 / 255.0, 50);
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
		
	Widget GetLayoutRoot()
	{
		return m_LayoutRoot;
	}
}