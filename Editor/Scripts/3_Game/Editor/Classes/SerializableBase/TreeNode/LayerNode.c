class LayerNode: NamedNode
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
	
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.TOGGLE;
	}
		
	override NodeState GetStateMask()
	{
		return NodeState.HOVER | NodeState.ACTIVE | NodeState.CONTEXT | NodeState.DRAG | NodeState.EXTEND;
	}
}