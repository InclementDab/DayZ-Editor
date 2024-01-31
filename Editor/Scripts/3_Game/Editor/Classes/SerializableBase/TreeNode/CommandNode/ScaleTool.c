class ScaleTool: CommandNode
{
	override array<string> GetXorSelections()
	{
		return { "Translate", "Rotate" };
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.TOGGLE;
	}
}