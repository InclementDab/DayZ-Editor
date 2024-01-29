class RotateTool: ToolNode
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