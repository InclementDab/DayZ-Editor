class ScaleTool: CommandNode
{
	override array<string> GetXorSelections()
	{
		return { "Translate", "Rotate" };
	}
	
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.TOGGLE;
	}
}