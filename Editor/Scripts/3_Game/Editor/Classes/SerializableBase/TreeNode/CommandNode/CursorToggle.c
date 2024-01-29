class CursorToggle: CommandNode
{
	override void OnSelectionChanged(bool state)
	{
		GetGame().GetUIManager().ShowCursor(state);
		if (GetEditor().GetHud()) {
			GetEditor().GetHud().ClearCursor();
		}
	}
	
	override ShortcutKeyType GetShortcutType()
	{
		return ShortcutKeyType.TOGGLE;
	}
}