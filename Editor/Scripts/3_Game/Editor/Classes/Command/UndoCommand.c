[RegisterCommand(UndoCommand)]
class UndoCommand: Command
{
	override void Execute(bool state) 
	{
		GetEditor().Undo();
	}
	
	override bool CanExecute()
	{
		return GetEditor().CanUndo();
	}
}