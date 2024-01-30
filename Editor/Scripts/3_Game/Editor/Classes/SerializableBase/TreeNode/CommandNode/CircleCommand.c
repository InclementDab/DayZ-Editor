class EllipseCommand: CommandNode
{
	override void SetState(TreeNodeState state)
	{
		if (state && GetEditor().GetHud()) {
			GetEditor().GetHud().CurrentSelectionMode = SelectionMode.ELLIPSE;
		}
	}
	
	override array<string> GetXorSelections()
	{
		return { "Box", "Lasso" };
	}
}