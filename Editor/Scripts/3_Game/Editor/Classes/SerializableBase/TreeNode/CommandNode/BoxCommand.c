class BoxCommand: CommandNode
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
		
		if (state && state && DaysBefore.GetEditor().GetHud()) {
			DaysBefore.GetEditor().GetHud().CurrentSelectionMode = SelectionMode.BOX;
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
}