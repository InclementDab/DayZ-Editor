class UndoCommand: CommandNode
{
	override void SetSelected(bool state) 
	{
		GetDayZGame().GetEditor().GetHud().ShowNotification("Test");
		GetDayZGame().GetEditor().Undo();
	}
		
	override bool CanSelect()
	{
		return GetDayZGame().GetEditor().CanUndo();
	}
}