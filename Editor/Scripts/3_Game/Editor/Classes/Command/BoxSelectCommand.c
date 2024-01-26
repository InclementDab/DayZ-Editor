[RegisterCommand(BoxSelectCommand, "Box Selection")]
class BoxSelectCommand: Command
{
	override void Execute(bool state)
	{
		super.Execute(state);
				
		if (state) {
			GetDayZGame().GetEditor().GetHud().CurrentSelectionMode = SelectionMode.BOX;
			GetDayZGame().GetCommand(CircleSelectCommand).Execute(false);
			GetDayZGame().GetCommand(LassoSelectCommand).Execute(false);
		}
	}
		
	override bool GetDefaultState()
	{
		return true;
	}
}
