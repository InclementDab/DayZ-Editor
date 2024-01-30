class EditorButton: ScriptedWidgetEventHandler
{		
	reference string Node;
	
	protected CommandNode m_Node;
	protected Widget m_LayoutRoot;
	
	TextWidget Text;
	ButtonWidget Button;
	ImageWidget Icon;
	
	void OnWidgetScriptInit(Widget w)
	{
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
	
#ifdef WORKBENCH
		if (!GetDayZGame()) {
			return;
		}
#endif
		
		Button = FindWidget<ButtonWidget>.SearchDown(m_LayoutRoot, "Button");				
		Button.SetHandler(this);
		Icon = FindWidget<ImageWidget>.SearchDown(m_LayoutRoot, "Icon");		
		Text = FindWidget<TextWidget>.SearchDown(m_LayoutRoot, "Text");
		
		if (Node == string.Empty) {
			return;
		}
		
		m_Node = CommandNode.Cast(GetDayZGame().GetEditor().GetNode(Node));
		if (!m_Node) {
			Error(string.Format("Could not find node %1", Node));
			return;
		}

		m_Node.State_OnChanged.Insert(OnStateChanged);
		OnStateChanged(m_Node, m_Node.GetDefaultState());
	}
		
	void OnStateChanged(TreeNode node, TreeNodeState state)
	{
		switch (state) {
			case TreeNodeState.EMPTY: {
				m_Node.GetEditor().GetHud().ClearCursor();
				//WidgetAnimator.AnimateColor(Icon, ARGB(100, 255, 255, 255), 50);
				Icon.LoadImageFile(0, m_Node.GetIcon().Regular());
				Icon.SetImage(0);
				break;
			}
			
			case TreeNodeState.HOVER: {
				WidgetAnimator.Animate(Icon, WidgetAnimatorProperty.COLOR_A, 1.0, 50);
				m_Node.GetEditor().GetHud().SetCursor(m_Node.GetIcon(), m_Node.GetDisplayName(), m_Node.GetShortcutString());		
				break;
			}
			
			case TreeNodeState.ACTIVE: {
				m_Node.GetEditor().GetHud().ClearCursor();
				//WidgetAnimator.AnimateColor(Icon, m_LayoutRoot.GetColor(), 50);
				Icon.LoadImageFile(0, m_Node.GetIcon().Solid());
				Icon.SetImage(0);
				break;
			}
		}
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_Node) {
			return false;
		}
		
		switch (m_Node.GetShortcutType()) {
			case ShortcutKeyType.HOLD: {
				m_Node.SetState(TreeNodeState.ACTIVE);
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
				m_Node.SetState(TreeNodeState.EMPTY);
				return true;
			}
			
			case ShortcutKeyType.PRESS: {
				m_Node.SetState(TreeNodeState.ACTIVE);
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
				m_Node.ToggleState();
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
				m_Node.ToggleState();
				return true;
			}
		}
		
		return false;
	}
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_Node) {
			m_Node.SetState(TreeNodeState.HOVER);
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_Node && m_Node.GetState() == TreeNodeState.HOVER) {
			m_Node.SetState(TreeNodeState.EMPTY);
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
		
	Widget GetLayoutRoot()
	{
		return m_LayoutRoot;
	}
}