class PlaceableNode: TreeNode
{
	override bool CreateContextMenu(inout ObservableCollection<ref ScriptView> list_items)
	{
		Editor editor = GetDayZGame().GetEditor();
		list_items.Insert(new CommandMenuItem(editor.GetCommand("Cut")));
		list_items.Insert(new CommandMenuItem(editor.GetCommand("Copy")));
		list_items.Insert(new CommandMenuItem(editor.GetCommand("Paste")));
		list_items.Insert(new CommandMenuDivider());
		
		return true;
	}
	
	override bool SetState(TreeNodeState state)
	{
		if (!super.SetState(state)) {
			return false;
		}
		
		vector matrix[4];
		Math3D.MatrixIdentity4(matrix);
		if (state) {
			GetEditor().GetNode(Editor.PLACING).Add(new ObjectNode(UUID.Generate(), m_UUID, GetIcon(), Editor.CreateObject(GetUUID(), matrix), EFE_DEFAULT));
			GetUApi().SupressNextFrame(true);
		}
		
		return true;
	}
}