class LassoSelectCommand: CommandNode
{
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
				
		if (selected) {
			GetDayZGame().GetEditor().GetHud().CurrentSelectionMode = SelectionMode.LASSO;
			GetDayZGame().GetEditor().GetCommand("CircleSelect").SetSelected(false);
			GetDayZGame().GetEditor().GetCommand("BoxSelect").SetSelected(false);
		}
	}
}