class ControlToggle: CommandNode
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);

		if (node_state.IsActive()) {
			if (state) {				
				DaysBefore.GetEditor().AddState(NodeState.ACTIVE);
			} else {
				DaysBefore.GetEditor().RemoveState(NodeState.ACTIVE);
			}
		}
	}
	
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.TOGGLE;
	}
}