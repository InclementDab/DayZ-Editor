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
	
	override void OnSelectionChanged(bool state)
	{
		super.OnSelectionChanged(state);
		
		if (state) {			
			Raycast raycast = GetDayZGame().GetEditor().GetCamera().PerformCursorRaycast();
			
			vector matrix[4];
			Math3D.MatrixIdentity4(matrix);
			matrix[3] = raycast.Bounce.Position;
			
			Object object = Editor.CreateObject(GetUUID(), matrix);
			ObjectNode editor_object = new ObjectNode(UUID.Generate(), m_UUID, GetIcon(), object, EFE_DEFAULT);
			if (object) {
				ObjectNode.ByObject[object] = editor_object;
			}
			
			GetDayZGame().GetEditor().Placing.Insert(editor_object);
		} else {
			GetDayZGame().GetEditor().Placing.Clear();
		}
	}
}