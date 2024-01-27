class RedoCommand: CommandNode
{
	override void SetSelected(bool state) 
	{
		GetDayZGame().GetEditor().Redo();
	}
	
	override bool CanSelect()
	{
		return GetDayZGame().GetEditor().CanRedo();
	}
}