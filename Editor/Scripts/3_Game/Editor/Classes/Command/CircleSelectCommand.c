[RegisterCommand(CircleSelectCommand)]
class CircleSelectCommand: Command
{
	override void Execute(bool state)
	{
		super.Execute(state);
				
		if (state) {
			GetDayZGame().GetEditor().GetHud().CurrentSelectionMode = SelectionMode.ELLIPSE;
			GetDayZGame().GetCommand(BoxSelectCommand).Execute(false);
			GetDayZGame().GetCommand(LassoSelectCommand).Execute(false);
		}
	}
	
	override string GetName()
	{
		return "Circle Selection";
	}
}