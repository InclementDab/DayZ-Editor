class CircleSelectCommand: CommandNode
{
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
				
		if (selected) {
			GetDayZGame().GetEditor().GetHud().CurrentSelectionMode = SelectionMode.ELLIPSE;
			GetDayZGame().GetEditor().GetCommand("BoxSelect").SetSelected(false);
			GetDayZGame().GetEditor().GetCommand("LassoSelect").SetSelected(false);
		}
	}
}