class LassoCommand: CommandNode
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
		
		if (node_state.IsActive() && state && GetEditor().GetHud()) {
			GetEditor().GetHud().CurrentSelectionMode = SelectionMode.LASSO;
		}
	}
	
	override array<string> GetXorSelections()
	{
		return { "Box", "Ellipse" };
	}
	
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.PRESS;
	}
}