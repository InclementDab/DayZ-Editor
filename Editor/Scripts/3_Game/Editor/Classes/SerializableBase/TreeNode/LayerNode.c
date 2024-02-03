class LayerNode: TreeNode
{
	override bool CreateContextMenu(inout ObservableCollection<ref MenuNode> list_items)
	{
		EditorNode editor = GetEditor();
		list_items.Insert(new MenuNode(editor.GetCommand("Cut")));
		list_items.Insert(new MenuNode(editor.GetCommand("Copy")));
		list_items.Insert(new MenuNode(editor.GetCommand("Paste")));
		list_items.Insert(new DividerView(null));
		
		list_items.Insert(new MenuNode(editor.GetCommand("AddLayer")));
		list_items.Insert(new MenuNode(editor.GetCommand("SetLayerActive")));
		
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