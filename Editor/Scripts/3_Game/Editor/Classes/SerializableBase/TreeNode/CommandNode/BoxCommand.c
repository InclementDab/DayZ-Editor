class BoxCommand: CommandNode
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
		
		if (state && state && GetEditor().GetHud()) {
			GetEditor().GetHud().CurrentSelectionMode = SelectionMode.BOX;
		}
	}
	
	override array<string> GetXorSelections()
	{
		return { "Lasso", "Ellipse" };
	}
	
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.PRESS;
	}
	
	override NodeState GetDefaultState()
	{
		return NodeState.ACTIVE;
	}
}