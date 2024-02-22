class EllipseCommand: CommandNode
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
		
		if (state && state && DaysBefore.GetEditor().GetHud()) {
			DaysBefore.GetEditor().GetHud().CurrentSelectionMode = SelectionMode.ELLIPSE;
		}
	}
	
	override array<string> GetXorSelections()
	{
		return { "Box", "Lasso" };
	}
	
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.PRESS;
	}
}