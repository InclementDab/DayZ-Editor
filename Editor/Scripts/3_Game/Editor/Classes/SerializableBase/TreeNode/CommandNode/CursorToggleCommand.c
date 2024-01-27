class CursorToggleCommand: CommandNode
{
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
		
		GetGame().GetUIManager().ShowCursor(selected);
	}
	
	override ShortcutKeyType GetShortcutType()
	{
		return ShortcutKeyType.TOGGLE;
	}
}