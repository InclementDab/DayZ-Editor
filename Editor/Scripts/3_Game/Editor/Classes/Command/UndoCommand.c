[RegisterCommand(UndoCommand)]
class UndoCommand: Command
{
	override void Execute(bool state) 
	{
		GetEditor().GetHud().ShowNotification("Undoing your mom");
		GetEditor().Undo();
	}
	
	override bool CanExecute()
	{
		return GetEditor().CanUndo();
	}
}