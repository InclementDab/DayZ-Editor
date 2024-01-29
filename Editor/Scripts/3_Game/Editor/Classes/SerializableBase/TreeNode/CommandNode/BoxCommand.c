class BoxCommand: CommandNode
{
	override void OnSelectionChanged(bool state)
	{
		if (state && GetEditor().GetHud()) {
			GetEditor().GetHud().CurrentSelectionMode = SelectionMode.BOX;
		}
	}
	
	override array<string> GetXorSelections()
	{
		return { "Lasso", "Ellipse" };
	}
}