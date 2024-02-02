class RenameCommand: CommandNode
{
	override void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
		super.OnStateChanged(state, total_state);
		
		if (state.IsActive() && total_state.IsActive()) {			
			foreach (TreeNode node: TreeNode.StateMachine[TreeNodeState.ACTIVE]) {
				node.View.EnableRename();
			}
		}
	}
		
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.PRESS;
	}
}