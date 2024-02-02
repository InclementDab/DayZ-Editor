class ControlToggle: CommandNode
{
	override void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
		super.OnStateChanged(state, total_state);

		if (state.IsActive()) {
			if (total_state.IsActive()) {				
				GetEditor().AddState(TreeNodeState.ACTIVE);
			} else {
				GetEditor().RemoveState(TreeNodeState.ACTIVE);
			}
		}
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.TOGGLE;
	}
}