class TranslateTool: CommandNode
{	
	override array<string> GetXorSelections()
	{
		return { "Scale", "Rotate" };
	}
	
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.TOGGLE;
	}
}