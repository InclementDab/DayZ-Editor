class FolderNode: TreeNode
{
	override bool CreateContextMenu(inout ObservableCollection<ref ScriptView> list_items)
	{
		EditorNode editor = GetDayZGame().GetEditor();
		list_items.Insert(new MenuItemView(editor.GetCommand("Cut")));
		list_items.Insert(new MenuItemView(editor.GetCommand("Copy")));
		list_items.Insert(new MenuItemView(editor.GetCommand("Paste")));
		list_items.Insert(new DividerView());
		
		list_items.Insert(new MenuItemView(editor.GetCommand("AddLayer")));
		list_items.Insert(new MenuItemView(editor.GetCommand("SetLayerActive")));
		
		return true;
	}
	override TreeNodeState GetStateMask()
	{
		return TreeNodeState.HOVER | TreeNodeState.ACTIVE | TreeNodeState.CONTEXT | TreeNodeState.DRAGGING;
	}
}