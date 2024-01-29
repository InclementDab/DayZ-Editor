class EllipseCommand: CommandNode
{
	override void OnSelectionChanged(bool state)
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