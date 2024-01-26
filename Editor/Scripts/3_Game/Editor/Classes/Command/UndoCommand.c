[RegisterCommand(UndoCommand, "Undo")]
class UndoCommand: Command
{
	override void Execute(bool state) 
	{
		GetDayZGame().GetEditor().GetHud().ShowNotification("Test");
		GetDayZGame().GetEditor().Undo();
	}
	
	override bool CanExecute()
	{
		return GetDayZGame().GetEditor().CanUndo();
	}
}