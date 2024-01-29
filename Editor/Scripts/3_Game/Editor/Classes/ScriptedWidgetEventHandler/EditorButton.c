class EditorButton: ScriptedWidgetEventHandler
{	
	protected CommandNode m_Node;
	
	reference string Node;
	
	protected Symbols m_Icon;
	protected Widget m_LayoutRoot;
	protected ButtonWidget m_Button;
	protected ImageWidget m_IconWidget;
	
	void OnWidgetScriptInit(Widget w)
	{
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
	
		m_Button = FindWidget<ButtonWidget>.SearchDown(m_LayoutRoot, "Button");
		if (!m_Button) {
			Error("Button widget not found below " + m_LayoutRoot.GetName());
			return;
		}
		
		m_Button.SetHandler(this);
		
		m_IconWidget = FindWidget<ImageWidget>.SearchDown(m_LayoutRoot, "Icon");
		if (!m_IconWidget) {
			Error("Icon widget not found below " + m_LayoutRoot.GetName());
			return;
		}
		
#ifdef WORKBENCH
		if (!GetDayZGame()) {
			return;
		}
#endif
	
		// This is executing too early because GetEditor hasnt actually been inserted yet!, what doo?>????	
		if (Node != string.Empty) {	
			m_Node = GetDayZGame().GetEditor().GetCommand(Node);
			if (m_Node) {
				m_Icon = m_Node.GetIcon();
				//m_Node.OnSelectionChanged.Insert(OnExecuted);
				GetDayZGame().GetEditor().Select(m_Node);
				//OnExecuted(m_Node);
			}
		}
	}
	/*
	
	void OnExecuted(TreeNode node)
	{
		SymbolSize size = Ternary<SymbolSize>.If(node.IsSelected(), SymbolSize.SOLID, SymbolSize.REGULAR);
		int color = Ternary<int>.If(node.IsSelected(), m_LayoutRoot.GetColor(),	ARGB(100, 255, 255, 255));
		
		WidgetAnimator.AnimateColor(m_IconWidget, color, 50);
		
		m_IconWidget.LoadImageFile(0, Ternary<Symbol>.If(node.IsSelected(), m_Icon.Solid(), m_Icon.Regular()));
		m_IconWidget.SetImage(0);
	}*/
		
	Widget GetLayoutRoot()
	{
		return m_LayoutRoot;
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
		
		if (m_Node) {
			GetDayZGame().GetEditor().GetHud().SetCursor(m_Icon, m_Node.GetDisplayName(), m_Node.GetShortcutString());
		}
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{				
		GetDayZGame().GetEditor().GetHud().ClearCursor();
		/*
		if (!m_Node.IsSelected()) {
			WidgetAnimator.Animate(m_IconWidget, WidgetAnimatorProperty.COLOR_A, 100.0 / 255.0, 50);
		}*/
		
		return true;
	}
}