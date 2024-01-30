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
		
		
		switch (state) {
			case TreeNodeState.ACTIVE: {
				vector matrix[4];
				Math3D.MatrixIdentity4(matrix);
				if (state) {
					GetEditor().GetNode(Editor.PLACING).Add(new ObjectNode(UUID.Generate(), m_UUID, GetIcon(), Editor.CreateObject(GetUUID(), matrix), EFE_DEFAULT));
					GetUApi().SupressNextFrame(true);
				}
				
				return true;
			}
		}

		return true;
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