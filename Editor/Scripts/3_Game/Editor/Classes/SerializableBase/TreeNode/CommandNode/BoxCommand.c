class BoxCommand: CommandNode
{
	override bool SetState(TreeNodeState state)
	{
		if (!super.SetState(state)) {
			return false;
		}
		
		if (state && GetEditor().GetHud()) {
			GetEditor().GetHud().CurrentSelectionMode = SelectionMode.BOX;
		}
		
		return true;
	}
	
	override array<string> GetXorSelections()
	{
		return { "Lasso", "Ellipse" };
	}
}