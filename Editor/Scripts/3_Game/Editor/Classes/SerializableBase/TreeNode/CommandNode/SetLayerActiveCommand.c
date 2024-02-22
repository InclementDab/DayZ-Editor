class SetLayerActiveCommand: CommandNode
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
		
		if (node_state.IsActive() && state) {
			foreach (Node tree_node_active: Node.States[NodeState.ACTIVE]) {
				LayerNode folder_node_active = LayerNode.Cast(tree_node_active);
				if (!folder_node_active) {
					continue;
				}
				
				folder_node_active.RemoveState(NodeState.ACTIVE);
			}
			
			foreach (Node tree_node: Node.States[NodeState.CONTEXT]) {
				LayerNode folder_node = LayerNode.Cast(tree_node);
				if (!folder_node) {
					continue;
				}
				
				folder_node.AddState(NodeState.ACTIVE);
			}
		}
	}
		
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.PRESS;
	}
}