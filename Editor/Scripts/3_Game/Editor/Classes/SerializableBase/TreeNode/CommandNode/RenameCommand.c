class RenameCommand: CommandNode
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(total_state, state);
		
		if (state.IsActive() && total_state.IsActive()) {			
			foreach (TreeNode node: TreeNode.StateMachine[NodeState.ACTIVE]) {
				node.View.EnableRename();
			}
		}
	}
		
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.PRESS;
	}
}