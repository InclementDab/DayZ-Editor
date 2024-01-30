class HudToggleCommand: CommandNode
{
	override void OnStateChanged(TreeNodeState state)
	{
		super.OnStateChanged(state);
		
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