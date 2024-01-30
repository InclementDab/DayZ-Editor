class HudToggleCommand: CommandNode
{
	override void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
		super.OnStateChanged(state, total_state);
		
		EditorHud hud = GetEditor().GetHud();
		if (hud) {
			hud.Show(m_TreeNodeState.IsActive());
		}
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.TOGGLE;
	}
}