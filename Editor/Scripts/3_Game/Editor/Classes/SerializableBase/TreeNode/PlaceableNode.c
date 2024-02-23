class PlaceableNode: NamedNode
{	
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
	
		EditorNode editor = EditorNode.Cast(FindAncestor(EditorNode));
		
		if (node_state.IsActive() && state) {	
			vector matrix[4];
			Math3D.MatrixIdentity4(matrix);
			editor.GetPlacing().Add(new ObjectNode(UUID.Generate(), m_UUID, Icon, LinearColor.WHITE, EditorNode.CreateObject(GetUUID(), matrix)));

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
		
		if (interact_type & NodeInteractType.PRESS) {
			ToggleState(NodeState.ACTIVE);
		}
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.PRESS | NodeInteractType.CONTEXT | NodeInteractType.DRAG_START | NodeInteractType.DRAG | NodeInteractType.DROP | NodeInteractType.ENTER | NodeInteractType.LEAVE;
	}
}