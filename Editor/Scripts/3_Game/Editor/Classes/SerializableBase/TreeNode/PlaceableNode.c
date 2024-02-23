class PlaceableNode: NamedNode
{	
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
	
		EditorNode editor = EditorNode.Cast(FindAncestor(EditorNode));
		
		if (node_state.IsActive()) {	
			if (state) {
				vector matrix[4];
				Math3D.MatrixIdentity4(matrix);
				editor.GetPlacing().Add(new ObjectNode(UUID.Generate(), m_UUID, Icon, LinearColor.WHITE, EditorNode.CreateObject(GetUUID(), matrix)));
			} else {
				foreach (Node node: DaysBefore.GetEditor().GetPlacing().Children) {
					ObjectNode object_node = ObjectNode.Cast(node);
					editor.InsertHistory(object_node, null);
					editor.GetPlacingDestination().Add(object_node);	
					editor.GetPlacing().Remove(object_node);
					
					object_node.AddState(NodeState.ACTIVE);
					object_node.SetSynchDirty();
					
					// remove it from placing
					editor.PlaySound(EditorSounds.PLOP);
					
					if (KeyState(KeyCode.KC_LSHIFT)) {
						AddState(NodeState.ACTIVE);
					}					
				}
			}
			
			GetUApi().SupressNextFrame(true);
		}
	}
	
	override NodeState GetStateMask()
	{
		return NodeState.ACTIVE | NodeState.VIEW_TREE | NodeState.CLIENT_AUTH | NodeState.SYNC_DIRTY;
	}

	override void OnInteract(NodeInteractType interact_type)
	{
		super.OnInteract(interact_type);
		
		ToggleState(NodeState.ACTIVE);
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.PRESS | NodeInteractType.CONTEXT | NodeInteractType.DRAG_START | NodeInteractType.DRAG | NodeInteractType.DROP | NodeInteractType.ENTER | NodeInteractType.LEAVE;
	}
}