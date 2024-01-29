class ScaleTool: CommandNode
{
	override array<string> GetXorSelections()
	{
		return { "Translate", "Rotate" };
	}
	
	override ShortcutKeyType GetShortcutType()
	{
		return ShortcutKeyType.TOGGLE;
	}
}