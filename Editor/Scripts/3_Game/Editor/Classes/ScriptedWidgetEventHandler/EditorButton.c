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
		
		if (Node != string.Empty) {
			m_Node = CommandNode.Cast(GetDayZGame().GetEditor().GetNode(Node));
			if (m_Node) {
				m_Node.State_OnChanged.Insert(OnStateChanged);
				OnStateChanged(m_Node, m_Node.GetDefaultState());
			}
		}
	}
		
	void OnStateChanged(TreeNode node, TreeNodeState state)
	{		
		if (!node.GetEditor().GetHud() || !Icon) {
			return;
		}
		
		switch (state) {
			case TreeNodeState.EMPTY: {
				node.GetEditor().GetHud().ClearCursor();
				//WidgetAnimator.AnimateColor(Icon, ARGB(100, 255, 255, 255), 50);
				Icon.LoadImageFile(0, node.GetIcon().Regular());
				Icon.SetImage(0);
				break;
			}
			
			case TreeNodeState.HOVER: {
				WidgetAnimator.Animate(Icon, WidgetAnimatorProperty.COLOR_A, 1.0, 50);
				node.GetEditor().GetHud().SetCursor(node.GetIcon(), node.GetDisplayName(), node.GetShortcutString());		
				break;
			}
			
			case TreeNodeState.ACTIVE: {
				node.GetEditor().GetHud().ClearCursor();
				//WidgetAnimator.AnimateColor(Icon, m_LayoutRoot.GetColor(), 50);
				Icon.LoadImageFile(0, node.GetIcon().Solid());
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
		
		switch (m_Node.GetInteractType()) {
			case TreeNodeInteract.HOLD: {
				m_Node.AddState(TreeNodeState.ACTIVE);
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
		
		switch (m_Node.GetInteractType()) {
			case TreeNodeInteract.HOLD: {
				m_Node.RemoveState(TreeNodeState.EMPTY);
				return true;
			}
			
			case TreeNodeInteract.PRESS: {
				m_Node.AddState(TreeNodeState.ACTIVE);
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
		
		switch (m_Node.GetInteractType()) {
			case TreeNodeInteract.TOGGLE: {
				if (m_Node.HasState(TreeNodeState.ACTIVE)) {
					m_Node.RemoveState(TreeNodeState.ACTIVE);
				} else {
					m_Node.AddState(TreeNodeState.ACTIVE);
				}
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
		
		switch (m_Node.GetInteractType()) {
			case TreeNodeInteract.DOUBLE: {
				if (m_Node.HasState(TreeNodeState.ACTIVE)) {
					m_Node.RemoveState(TreeNodeState.ACTIVE);
				} else {
					m_Node.AddState(TreeNodeState.ACTIVE);
				}
				return true;
			}
		}
		
		return false;
	}
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_Node) {
			m_Node.AddState(TreeNodeState.HOVER);
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_Node) {
			m_Node.RemoveState(TreeNodeState.EMPTY);
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
		
	Widget GetLayoutRoot()
	{
		return m_LayoutRoot;
	}
}