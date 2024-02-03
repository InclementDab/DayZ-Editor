class LayerNode: TreeNode
{
	override bool CreateContextMenu(inout ObservableCollection<ref MenuNode> list_items)
	{
		list_items.Insert(new MenuNode(TreeNode.ROOT.GetCommand("Cut")));
		list_items.Insert(new MenuNode(TreeNode.ROOT.GetCommand("Copy")));
		list_items.Insert(new MenuNode(TreeNode.ROOT.GetCommand("Paste")));
		list_items.Insert(new DividerView(null));
		
		list_items.Insert(new MenuNode(TreeNode.ROOT.GetCommand("AddLayer")));
		list_items.Insert(new MenuNode(TreeNode.ROOT.GetCommand("SetLayerActive")));
		
		return true;
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.TOGGLE;
	}
	
	override bool CanDelete()
	{
		return true;
	}
		
	override TreeNodeState GetStateMask()
	{
		return TreeNodeState.HOVER | TreeNodeState.ACTIVE | TreeNodeState.CONTEXT | TreeNodeState.DRAGGING | TreeNodeState.FOCUS | TreeNodeState.EXTEND;
	}
}