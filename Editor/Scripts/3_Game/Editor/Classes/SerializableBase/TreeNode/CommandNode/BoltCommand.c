class BoltCommand: CommandNode
{
	override ShortcutKeyType GetShortcutType()
	{
		return ShortcutKeyType.TOGGLE;
	}
}