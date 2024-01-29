class BoxCommand: CommandNode
{
	override array<string> GetXorSelections()
	{
		return { "Lasso", "Circle" };
	}
}