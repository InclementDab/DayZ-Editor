class RotateTool: CommandNode
{
	override array<string> GetXorSelections()
	{
		return { "Scale", "Translate" };
	}
	
	override ShortcutKeyType GetShortcutType()
	{
		return ShortcutKeyType.TOGGLE;
	}
}