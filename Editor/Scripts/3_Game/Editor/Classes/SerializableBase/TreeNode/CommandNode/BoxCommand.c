class BoxCommand: CommandNode
{
	override void OnStateChanged(TreeNodeState state)
	{
		super.OnStateChanged(state);
		
		if (state.IsActive() && GetEditor().GetHud()) {
			GetEditor().GetHud().CurrentSelectionMode = SelectionMode.BOX;
		}
	}
	
	override array<string> GetXorSelections()
	{
		return { "Lasso", "Ellipse" };
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.PRESS;
	}
	
	override TreeNodeState GetDefaultState()
	{
		return TreeNodeState.ACTIVE;
	}
}