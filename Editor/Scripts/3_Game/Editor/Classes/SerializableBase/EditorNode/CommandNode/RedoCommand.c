[RegisterCommand(RedoCommand, "Redo", Symbols.ROTATE_RIGHT)]
class RedoCommand: EditorNode
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