class RotateTool: ToolNode
{
	override array<string> GetXorSelections()
	{
		return { "Scale", "Translate" };
	}
}