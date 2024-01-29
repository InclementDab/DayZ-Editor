class CircleCommand: CommandNode
{
	override array<string> GetXorSelections()
	{
		return { "Box", "Lasso" };
	}
}