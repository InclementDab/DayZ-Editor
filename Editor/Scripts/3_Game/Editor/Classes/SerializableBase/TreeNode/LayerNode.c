class LayerNode: TreeNode
{
	override bool CreateContextMenu(inout ObservableCollection<ref MenuNode> list_items)
	{
		list_items.Insert(new MenuNode(GetDayZGame().GetSandbox().GetCommand("Cut")));
		list_items.Insert(new MenuNode(GetDayZGame().GetSandbox().GetCommand("Copy")));
		list_items.Insert(new MenuNode(GetDayZGame().GetSandbox().GetCommand("Paste")));
		list_items.Insert(new DividerView(null));
		
		list_items.Insert(new MenuNode(GetDayZGame().GetSandbox().GetCommand("AddLayer")));
		list_items.Insert(new MenuNode(GetDayZGame().GetSandbox().GetCommand("SetLayerActive")));
		
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