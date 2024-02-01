class NodeView: ScriptView
{
	protected TreeNode m_Node;
	
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
	
	override bool OnDrag(Widget w, int x, int y)
	{		
		m_Node.AddState(TreeNodeState.DRAGGING);
		return false;
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{		
		return false;
	}
	
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		Print(reciever);
		if (!RecursiveGetParent(reciever, "Root")) {
			return false;
		}
		
		Class user_data;
		reciever.GetUserData(user_data);
		NodeView node_view = NodeView.Cast(user_data);
		TreeNode node = node_view.GetNode();
		
		Print(reciever);
		
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