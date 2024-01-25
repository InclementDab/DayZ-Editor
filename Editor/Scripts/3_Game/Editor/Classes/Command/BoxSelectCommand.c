enum SelectionMode
{
	BOX,
	ELLIPSE,
	LASSO // ???? are you CRAZY?
};

[RegisterCommand(BoxSelectCommand)]
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
	
	override string GetName()
	{
		return "Box Selection";
	}
	
	override bool GetDefaultState()
	{
		return true;
	}
}
