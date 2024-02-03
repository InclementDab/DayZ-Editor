class PlaceableNode: TreeNode
{
	override bool CreateContextMenu(inout ObservableCollection<ref MenuNode> list_items)
	{
		return false;
	}
	
	override void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
		super.OnStateChanged(state, total_state);
	
		if (state.IsActive()) {	
			if (total_state.IsActive()) {
				vector matrix[4];
				Math3D.MatrixIdentity4(matrix);
				GetEditor().GetPlacing().Add(new ObjectNode(UUID.Generate(), m_UUID, GetIcon(), EditorNode.CreateObject(GetUUID(), matrix)));
				
				GetUApi().SupressNextFrame(true);
			} else {
				foreach (TreeNode node: GetEditor()[EditorNode.PLACING].Children) {
					ObjectNode object_node = ObjectNode.Cast(node);
					GetEditor().InsertHistory(string.Format("Undo Place %1", object_node.GetUUID()), Symbols.CLOCK_ROTATE_LEFT, object_node, null);
					GetEditor().GetPlacingDestination().Add(object_node);
					GetEditor().GetPlacingDestination().Synchronize();			
					GetEditor().GetPlacing().Remove(object_node);
					
					object_node.AddState(TreeNodeState.ACTIVE);
					
					// remove it from placing
					GetEditor().PlaySound(EditorSounds.PLOP);
					
					if (KeyState(KeyCode.KC_LSHIFT)) {
						AddState(TreeNodeState.ACTIVE);
					}
					
					GetUApi().SupressNextFrame(true);
				}
			}
		}
	}
	
	override TreeNodeState GetStateMask()
	{
		return TreeNodeState.HOVER | TreeNodeState.ACTIVE | TreeNodeState.CONTEXT | TreeNodeState.DRAGGING;
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.PRESS;
	}
}