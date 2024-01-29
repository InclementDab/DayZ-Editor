class LassoCommand: CommandNode
{
	override void OnSelectionChanged(bool state)
	{
		if (state && GetEditor().GetHud()) {
			GetEditor().GetHud().CurrentSelectionMode = SelectionMode.LASSO;
		}
	}
	
	override array<string> GetXorSelections()
	{
		return { "Box", "Ellipse" };
	}
}