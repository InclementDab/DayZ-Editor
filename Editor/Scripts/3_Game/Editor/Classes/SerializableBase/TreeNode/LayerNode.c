class LayerNode: NamedNode
{
	override bool CreateContextMenu(inout ObservableCollection<ref MenuNode> list_items)
	{
		list_items.Insert(new MenuNode(GetDayZGame().GetDaysBefore().GetCommand("Cut")));
		list_items.Insert(new MenuNode(GetDayZGame().GetDaysBefore().GetCommand("Copy")));
		list_items.Insert(new MenuNode(GetDayZGame().GetDaysBefore().GetCommand("Paste")));
		list_items.Insert(new DividerView(null));
		
		list_items.Insert(new MenuNode(GetDayZGame().GetDaysBefore().GetCommand("AddLayer")));
		list_items.Insert(new MenuNode(GetDayZGame().GetDaysBefore().GetCommand("SetLayerActive")));
		
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