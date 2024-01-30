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
	
	override void OnStateChanged(TreeNodeState state)
	{
		super.OnStateChanged(state);
	
		if (state.IsActive()) {			
			vector matrix[4];
			Math3D.MatrixIdentity4(matrix);

			ObjectNode node = new ObjectNode(UUID.Generate(), m_UUID, GetIcon(), Editor.CreateObject(GetUUID(), matrix), EFE_DEFAULT);
			//node.AddState(TreeNodeState.ACTIVE);
			GetEditor().GetNode(Editor.PLACING).Add(node);
			GetUApi().SupressNextFrame(true);
			
			// Clear all other states
			foreach (TreeNode active_node: TreeNode.StateMachine[TreeNodeState.ACTIVE]) {
				if (!active_node || !active_node.IsInherited(PlaceableNode)) {
					continue;
				}
				
				if (active_node == this) {
					continue;
				}
				
				active_node.RemoveState(TreeNodeState.ACTIVE);
			}
		}
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.PRESS;
	}
}

/*switch (button) {
					case 0: {
						if (!KeyState(KeyCode.KC_LSHIFT)) {
							m_Node.GetParent().Children.SetAllStates(TreeNodeState.EMPTY);
						}
						
						if (KeyState(KeyCode.KC_LCONTROL)) {
							m_Node.ToggleState();
						} else {
							m_Node.SetState(TreeNodeState.ACTIVE);
						}
						
						return true;
					}
					
					case 1: {
						EditorHud hud = GetDayZGame().GetEditor().GetHud();
						hud.GetTemplateController().MenuItems.Clear();
						
						if (m_Node.CreateContextMenu(hud.GetTemplateController().MenuItems)) {
							hud.Menu.Show(true);
							hud.Menu.SetScreenPos(x, y);
						} else {
							hud.Menu.Show(false);
						}
						
						return true;
					}
				}*/