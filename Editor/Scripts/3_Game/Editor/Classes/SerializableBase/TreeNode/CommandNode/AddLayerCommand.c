class AddLayerCommand: CommandNode
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(state, total_state);
		
		if (state.IsActive() && total_state.IsActive()) {			
			GetEditor().GetPlacingDestination().Add(new LayerNode(UUID.Generate(), "layer0", Symbols.FOLDER));
		}
	}
		
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.PRESS;
	}
}