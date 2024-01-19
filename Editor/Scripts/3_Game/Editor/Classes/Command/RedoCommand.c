[RegisterCommand(RedoCommand)]
class RedoCommand: Command
{
	override void Execute(bool state) 
	{
		GetEditor().Redo();
	}
	
	override bool CanExecute()
	{
		return GetEditor().CanRedo();
	}
}