class LassoCommand: CommandNode
{
	override bool SetState(TreeNodeState state)
	{
		if (!super.SetState(state)) {
			return false;
		}
		
		if (state && GetEditor().GetHud()) {
			GetEditor().GetHud().CurrentSelectionMode = SelectionMode.LASSO;
		}
		
		return true;
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