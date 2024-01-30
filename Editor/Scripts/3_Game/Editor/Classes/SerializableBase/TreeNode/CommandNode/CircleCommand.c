class EllipseCommand: CommandNode
{
	override void OnStateChanged(TreeNodeState state)
	{
		super.OnStateChanged(state);
		
		if (state.IsActive() && GetEditor().GetHud()) {
			GetEditor().GetHud().CurrentSelectionMode = SelectionMode.ELLIPSE;
		}
	}
	
	override array<string> GetXorSelections()
	{
		return { "Box", "Lasso" };
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.PRESS;
	}
}