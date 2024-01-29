class EditorButton: ScriptedWidgetEventHandler
{		
	reference string Node;
	
	protected Symbols m_Icon;
	protected Widget m_LayoutRoot;
	protected TextWidget m_TextWidget;
	protected ButtonWidget m_Button;
	protected ImageWidget m_IconWidget;
	protected TreeNode m_Node;
	
	void OnWidgetScriptInit(Widget w)
	{
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
	
		m_Button = FindWidget<ButtonWidget>.SearchDown(m_LayoutRoot, "Button");		
		m_Button.SetHandler(this);
		
		m_IconWidget = FindWidget<ImageWidget>.SearchDown(m_LayoutRoot, "Icon");		
		m_TextWidget = FindWidget<TextWidget>.SearchDown(m_LayoutRoot, "Text");
		
#ifdef WORKBENCH
		if (!GetDayZGame()) {
			return;
		}
#endif
		
		m_Node = GetDayZGame().GetEditor().GetNode(Node);
		if (m_Node) {
			m_Icon = m_Node.GetIcon();
			m_Node.AfterSelectionChanged.Insert(OnExecuted);
			GetDayZGame().GetEditor().Select(m_Node);
			OnExecuted(m_Node, m_Node.GetDefaultState());
		}
	}
	
	void OnExecuted(TreeNode node, bool state)
	{
		SymbolSize size = Ternary<SymbolSize>.If(state, SymbolSize.SOLID, SymbolSize.REGULAR);
		int color = Ternary<int>.If(state, m_LayoutRoot.GetColor(),	ARGB(100, 255, 255, 255));
		
		WidgetAnimator.AnimateColor(m_IconWidget, color, 50);
		
		m_IconWidget.LoadImageFile(0, Ternary<Symbol>.If(state, m_Icon.Solid(), m_Icon.Regular()));
		m_IconWidget.SetImage(0);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		WidgetAnimator.Animate(m_IconWidget, WidgetAnimatorProperty.COLOR_A, 1.0, 50);		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{				
		GetDayZGame().GetEditor().GetHud().ClearCursor();		
		return true;
	}
		
	Widget GetLayoutRoot()
	{
		return m_LayoutRoot;
	}
}