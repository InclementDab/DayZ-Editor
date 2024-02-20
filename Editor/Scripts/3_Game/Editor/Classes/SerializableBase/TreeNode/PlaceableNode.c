class PlaceableNode: SandboxNode
{	
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(state, total_state);
	
		if (state.IsActive()) {	
			if (total_state.IsActive()) {
				vector matrix[4];
				Math3D.MatrixIdentity4(matrix);
				GetEditor().GetPlacing().Add(new ObjectNode(UUID.Generate(), m_UUID, GetIcon(), EditorNode.CreateObject(GetUUID(), matrix)));
				
				GetUApi().SupressNextFrame(true);
			} else {
				foreach (SandboxNode node: GetEditor().GetPlacing().Children) {
					ObjectNode object_node = ObjectNode.Cast(node);
					GetEditor().InsertHistory(string.Format("Undo Place %1", object_node.GetUUID()), Symbols.CLOCK_ROTATE_LEFT, object_node, null);
					GetEditor().GetPlacingDestination().Add(object_node);
					GetEditor().GetPlacingDestination().Synchronize();			
					GetEditor().GetPlacing().Remove(object_node);
					
					object_node.AddState(NodeState.ACTIVE);
					
					// remove it from placing
					GetEditor().PlaySound(EditorSounds.PLOP);
					
					if (KeyState(KeyCode.KC_LSHIFT)) {
						AddState(NodeState.ACTIVE);
					}					
				}
				
				GetUApi().SupressNextFrame(true);
			}
		}
	}
	
	override NodeState GetStateMask()
	{
		return NodeState.HOVER | NodeState.ACTIVE | NodeState.CONTEXT | NodeState.DRAGGING;
	}
	
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.PRESS;
	}
}