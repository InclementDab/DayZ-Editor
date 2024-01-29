class LassoCommand: CommandNode
{
	override array<string> GetXorSelections()
	{
		return { "Box", "Circle" };
	}
}