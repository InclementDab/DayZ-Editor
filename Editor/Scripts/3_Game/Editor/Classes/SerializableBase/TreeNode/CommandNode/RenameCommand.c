class RenameCommand: CommandNode
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
		
		if (node_state.IsActive() && state) {			
			foreach (Node node: Node.States[NodeState.ACTIVE]) {
				//node.View.EnableRename();
			}
		}
	}
		
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.PRESS;
	}
}