class RotateTool: CommandNode
{
	override array<string> GetXorSelections()
	{
		return { "Scale", "Translate" };
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.TOGGLE;
	}
}