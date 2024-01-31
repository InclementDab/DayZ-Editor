class SetLayerActiveCommand: CommandNode
{
	override void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
		super.OnStateChanged(state, total_state);
		
		if (state.IsActive() && total_state.IsActive()) {
			foreach (TreeNode tree_node_active: TreeNode.StateMachine[TreeNodeState.ACTIVE]) {
				FolderNode folder_node_active = FolderNode.Cast(tree_node_active);
				if (!folder_node_active) {
					continue;
				}
				
				folder_node_active.RemoveState(TreeNodeState.ACTIVE);
			}
			
			foreach (TreeNode tree_node: TreeNode.StateMachine[TreeNodeState.CONTEXT]) {
				FolderNode folder_node = FolderNode.Cast(tree_node);
				if (!folder_node) {
					continue;
				}
				
				folder_node.AddState(TreeNodeState.ACTIVE);
			}
		}
	}
		
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.PRESS;
	}
}