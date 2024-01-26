[RegisterCommand(RedoCommand, "Redo")]
class RedoCommand: Command
{
	override void Execute(bool state) 
	{
		GetDayZGame().GetEditor().Redo();
	}
	
	override bool CanExecute()
	{
		return GetDayZGame().GetEditor().CanRedo();
	}
}