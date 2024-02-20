class RotateTool: CommandNode
{
	override array<string> GetXorSelections()
	{
		return { "Scale", "Translate" };
	}
	
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.TOGGLE;
	}
}