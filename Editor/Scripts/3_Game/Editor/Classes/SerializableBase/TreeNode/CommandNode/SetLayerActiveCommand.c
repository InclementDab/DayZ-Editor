class SetLayerActiveCommand: CommandNode
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(state, total_state);
		
		if (state.IsActive() && total_state.IsActive()) {
			foreach (TreeNode tree_node_active: TreeNode.StateMachine[NodeState.FOCUS]) {
				LayerNode folder_node_active = LayerNode.Cast(tree_node_active);
				if (!folder_node_active) {
					continue;
				}
				
				folder_node_active.RemoveState(NodeState.FOCUS);
			}
			
			foreach (TreeNode tree_node: TreeNode.StateMachine[NodeState.CONTEXT]) {
				LayerNode folder_node = LayerNode.Cast(tree_node);
				if (!folder_node) {
					continue;
				}
				
				folder_node.AddState(NodeState.FOCUS);
			}
		}
	}
		
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.PRESS;
	}
}