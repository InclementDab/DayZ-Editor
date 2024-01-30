class TranslateTool: CommandNode
{	
	override array<string> GetXorSelections()
	{
		return { "Scale", "Rotate" };
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.TOGGLE;
	}
}