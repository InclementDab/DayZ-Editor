class NodeView: ScriptView
{
	protected TreeNode m_Node;
	
	// Primary interaction point!!
	Widget Panel;
	
	void NodeView(TreeNode node)
	{
		m_Node = node;
		m_Node.State_OnChanged.Insert(OnStateChanged);
	}
	
	void OnStateChanged(TreeNode node, TreeNodeState state)
	{
	}
	
	TreeNode GetNode()
	{
		return m_Node;
	}
		
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button != 0) {
			return false;
		}
		
		switch (w) {
			case Panel: {
				switch (m_Node.GetInteractType()) {
					case TreeNodeInteract.HOLD: {
						m_Node.AddState(TreeNodeState.ACTIVE);
						return true;
					}
					
					case TreeNodeInteract.TOGGLE: {
						if (m_Node.HasState(TreeNodeState.ACTIVE)) {
							m_Node.RemoveState(TreeNodeState.ACTIVE);
						} else {
							m_Node.AddState(TreeNodeState.ACTIVE);
						}
						return true;
					}
					
					case TreeNodeInteract.PRESS: {
						m_Node.AddState(TreeNodeState.ACTIVE);
						return true;
					}
				}
				
				return true;
			}
		}
				
		return false;
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (button != 0) {
			return false;
		}
		
		switch (m_Node.GetInteractType()) {
			case TreeNodeInteract.HOLD: {
				m_Node.RemoveState(TreeNodeState.ACTIVE);
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
		
		switch (w) {
			case Panel: {
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
			}
		}
				
		return false;
	}
		
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (button != 0 || !m_Node) {
			return false;
		}
		
		switch (w) {
			case Panel: {
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
			}
		}
		
		return false;
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{		
		m_Node.AddState(TreeNodeState.DRAGGING);
		return false;
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{		
		return false;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_Node && w == Panel) {
			m_Node.AddState(TreeNodeState.HOVER);
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_Node && !m_Node.HasState(TreeNodeState.ACTIVE)) {
			m_Node.RemoveState(TreeNodeState.HOVER);
		}
				
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		if (!RecursiveGetParent(reciever, "Root")) {
			return false;
		}
		
		Class user_data;
		reciever.GetUserData(user_data);
		NodeView node_view = NodeView.Cast(user_data);
		TreeNode node = node_view.GetNode();		
		return false;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		m_Node.RemoveState(TreeNodeState.ACTIVE);
		m_Node.RemoveState(TreeNodeState.DRAGGING);
		return false;
	}
	
	override bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		if (!RecursiveGetParent(w, "Root")) {
			return false;
		}
		
		Class user_data;
		w.GetUserData(user_data);
		NodeView node_view = NodeView.Cast(user_data);
		TreeNode node = node_view.GetNode();
		delete node_view;		
		m_Node.Add(node);
		node.RemoveState(TreeNodeState.DRAGGING);
		return true;
	}	
}