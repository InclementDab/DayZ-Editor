class AddLayerCommand: CommandNode
{
	override void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
		super.OnStateChanged(state, total_state);
		
		if (state.IsActive() && total_state.IsActive()) {
			GetEditor().GetPlacingDestination().Add(new FolderNode(UUID.Generate(), "layer0", Symbols.FOLDER));
		}
	}
		
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.PRESS;
	}
}