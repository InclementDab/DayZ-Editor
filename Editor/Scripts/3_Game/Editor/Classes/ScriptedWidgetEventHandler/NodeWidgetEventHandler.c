class NodeWidgetEventHandler: ScriptedWidgetEventHandler
{
	reference string Node;
	protected TreeNode m_Node;
	
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
		Icon = FindWidget<ImageWidget>.SearchDown(m_LayoutRoot, "Icon");		
		Text = FindWidget<TextWidget>.SearchDown(m_LayoutRoot, "Text");		
		if (Node != string.Empty) {
			Print(Node);
			m_Node = TreeNode.ROOT[Node];
			Print(m_Node);
			if (m_Node) {
				if (Button) {
					Button.SetHandler(this);
				}
				
				if (Icon) {
					Icon.LoadImageFile(0, m_Node.GetIcon().Regular());
					Icon.SetImage(0);
				}
				
				m_Node.State_OnChanged.Insert(OnStateChanged);
				OnStateChanged(m_Node, m_Node.GetDefaultState());
			}
		}
	}
	
	void OnStateChanged(TreeNode node, TreeNodeState state)
	{
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
		if (!m_Node) {
			return false;
		}
		
		if (button == 1) {
			m_Node.AddState(TreeNodeState.CONTEXT);
			return true;
		}
		
		if (button == 0) {
			switch (m_Node.GetInteractType()) {
				case TreeNodeInteract.HOLD: {
					m_Node.RemoveState(TreeNodeState.ACTIVE);
					return true;
				}
				
				case TreeNodeInteract.ONCE:
				case TreeNodeInteract.PRESS: {
					m_Node.AddState(TreeNodeState.ACTIVE);									
					return true;
				}
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
			m_Node.RemoveState(TreeNodeState.HOVER);
		}
				
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	Widget GetLayoutRoot()
	{
		return m_LayoutRoot;
	}
}