class PlaceableNode: NamedNode
{	
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
	
		if (node_state.IsActive()) {	
			if (state) {
				vector matrix[4];
				Math3D.MatrixIdentity4(matrix);
				GetEditor().GetPlacing().Add(new ObjectNode(UUID.Generate(), m_UUID, Icon, LinearColor.WHITE, EditorNode.CreateObject(GetUUID(), matrix)));
				
				GetUApi().SupressNextFrame(true);
			} else {
				foreach (Node node: GetEditor().GetPlacing().Children) {
					ObjectNode object_node = ObjectNode.Cast(node);
					GetEditor().InsertHistory(object_node, null);
					GetEditor().GetPlacingDestination().Add(object_node);
					GetEditor().GetPlacingDestination().SetSynchDirty();			
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
		return NodeState.HOVER | NodeState.ACTIVE | NodeState.CONTEXT | NodeState.DRAG;
	}
	
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.PRESS;
	}
}