class EllipseCommand: CommandNode
{
	override bool SetState(TreeNodeState state)
	{
		if (!super.SetState(state)) {
			return false;
		}
		
		if (state && GetEditor().GetHud()) {
			GetEditor().GetHud().CurrentSelectionMode = SelectionMode.ELLIPSE;
		}
		
		return true;
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