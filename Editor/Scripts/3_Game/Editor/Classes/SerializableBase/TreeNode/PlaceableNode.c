class PlaceableNode: TreeNode
{
	protected string m_ObjectUuid;
	
	override bool CreateContextMenu(inout ObservableCollection<ref ScriptView> list_items)
	{
		Editor editor = GetDayZGame().GetEditor();
		list_items.Insert(new CommandMenuItem(editor.GetCommand("Cut")));
		list_items.Insert(new CommandMenuItem(editor.GetCommand("Copy")));
		list_items.Insert(new CommandMenuItem(editor.GetCommand("Paste")));
		list_items.Insert(new CommandMenuDivider());
		
		return true;
	}
	
	override void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
		super.OnStateChanged(state, total_state);
	
		if (state.IsActive()) {	
			if (total_state.IsActive() && m_ObjectUuid == string.Empty) {
				vector matrix[4];
				Math3D.MatrixIdentity4(matrix);
				m_ObjectUuid = UUID.Generate();
				ObjectNode node = new ObjectNode(m_ObjectUuid, m_UUID, GetIcon(), Editor.CreateObject(GetUUID(), matrix), EFE_DEFAULT);
				GetEditor().GetNode(Editor.PLACING).Add(node);
				
				GetUApi().SupressNextFrame(true);
			} else {
				ObjectNode object_node = GetEditor()[Editor.PLACING][m_ObjectUuid];
				m_ObjectUuid = string.Empty;
				GetEditor().InsertHistory(string.Format("Undo Place %1", object_node.GetUUID()), Symbols.CLOCK_ROTATE_LEFT, object_node, null);
				GetEditor()[Editor.PLACING].Remove(object_node);
				GetEditor()[Editor.EDITS].Add(object_node);
				GetEditor()[Editor.EDITS].Synchronize();
							
				object_node.AddState(TreeNodeState.ACTIVE);
				
				// remove it from placing
				GetEditor().PlaySound(EditorSounds.PLOP);
				
				if (KeyState(KeyCode.KC_LSHIFT)) {
					AddState(TreeNodeState.ACTIVE);
				}
			}
		}
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.PRESS;
	}
}