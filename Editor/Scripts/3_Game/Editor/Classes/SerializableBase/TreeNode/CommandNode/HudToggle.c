class HudToggle: CommandNode
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(state, total_state);
		
		EditorHud hud = GetEditor().GetHud();
		if (hud) {
			hud.Show(total_state.IsActive());
		}
	}
	
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.TOGGLE;
	}
}