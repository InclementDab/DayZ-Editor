class ScaleTool: ToolNode
{
	override array<string> GetXorSelections()
	{
		return { "Translate", "Rotate" };
	}
}