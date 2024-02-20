class ControlToggle: CommandNode
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(state, total_state);

		if (state.IsActive()) {
			if (total_state.IsActive()) {				
				GetEditor().AddState(NodeState.ACTIVE);
			} else {
				GetEditor().RemoveState(NodeState.ACTIVE);
			}
		}
	}
	
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.TOGGLE;
	}
}