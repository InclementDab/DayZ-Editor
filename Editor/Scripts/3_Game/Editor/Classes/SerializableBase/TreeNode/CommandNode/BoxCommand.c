class BoxCommand: CommandNode
{
	override bool SetState(TreeNodeState state)
	{
		if (!super.SetState(state)) {
			return false;
		}
		
		if (state.IsActive() && GetEditor().GetHud()) {
			GetEditor().GetHud().CurrentSelectionMode = SelectionMode.BOX;
		}
		
		return true;
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