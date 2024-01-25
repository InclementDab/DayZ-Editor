[RegisterCommand(LassoSelectCommand)]
class LassoSelectCommand: Command
{
	override void Execute(bool state)
	{
		super.Execute(state);
		
		if (state) {
			GetDayZGame().GetCommand(BoxSelectCommand).Execute(false);
			GetDayZGame().GetCommand(CircleSelectCommand).Execute(false);
		}
	}
	
	override string GetName()
	{
		return "Lasso Selection";
	}
}