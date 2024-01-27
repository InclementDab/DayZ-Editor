class BoxSelectCommand: CommandNode
{
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
				
		if (selected) {
			GetDayZGame().GetEditor().GetHud().CurrentSelectionMode = SelectionMode.BOX;
			GetDayZGame().GetEditor().GetCommand("CircleSelect").SetSelected(false);
			GetDayZGame().GetEditor().GetCommand("LassoSelect").SetSelected(false);
		}
	}
		
	override bool GetDefaultState()
	{
		return true;
	}
}
