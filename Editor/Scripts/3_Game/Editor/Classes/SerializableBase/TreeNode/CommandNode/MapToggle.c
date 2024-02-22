class MapToggle: CommandNode
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
		
		EditorHud hud = DaysBefore.GetEditor().GetHud();
		if (hud) {
			hud.Map.Show(state);
		}
	}
	
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.TOGGLE;
	}
}