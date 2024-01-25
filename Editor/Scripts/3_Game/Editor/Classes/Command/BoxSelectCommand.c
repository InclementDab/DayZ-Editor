[RegisterCommand(BoxSelectCommand)]
class BoxSelectCommand: Command
{
	override void Execute(bool state)
	{
		super.Execute(state);
		
		if (state) {
			GetDayZGame().GetCommand(CircleSelectCommand).Execute(false);
			GetDayZGame().GetCommand(LassoSelectCommand).Execute(false);
		}
	}
	
	override string GetName()
	{
		return "Box Selection";
	}
	
	override bool GetDefaultState()
	{
		return true;
	}
}
