class LassoCommand: CommandNode
{
	override void OnStateChanged(TreeNodeState state)
	{
		super.OnStateChanged(state);
		
		if (state.IsActive() && GetEditor().GetHud()) {
			GetEditor().GetHud().CurrentSelectionMode = SelectionMode.LASSO;
		}
	}
	
	override array<string> GetXorSelections()
	{
		return { "Box", "Ellipse" };
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.PRESS;
	}
}