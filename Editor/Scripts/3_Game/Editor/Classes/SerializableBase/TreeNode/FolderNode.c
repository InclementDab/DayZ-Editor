class FolderNode: TreeNode
{
	override bool CreateContextMenu(inout ObservableCollection<ref ScriptView> list_items)
	{
		EditorNode editor = GetDayZGame().GetEditor();
		list_items.Insert(new CommandMenuItem(editor.GetCommand("Cut")));
		list_items.Insert(new CommandMenuItem(editor.GetCommand("Copy")));
		list_items.Insert(new CommandMenuItem(editor.GetCommand("Paste")));
		list_items.Insert(new CommandMenuDivider());
		
		list_items.Insert(new CommandMenuItem(editor.GetCommand("NewFolder")));
		
		return true;
	}
	override TreeNodeState GetStateMask()
	{
		return TreeNodeState.HOVER | TreeNodeState.ACTIVE | TreeNodeState.CONTEXT | TreeNodeState.DRAGGING;
	}
}