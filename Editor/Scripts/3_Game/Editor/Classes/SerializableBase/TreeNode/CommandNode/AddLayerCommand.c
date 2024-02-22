class AddLayerCommand: CommandNode
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
		
		if (state && state) {			
			GetEditor().GetPlacingDestination().Add(new LayerNode(UUID.Generate(), "layer0", Symbols.FOLDER, LinearColor.WHITE));
		}
	}
		
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.PRESS;
	}
}