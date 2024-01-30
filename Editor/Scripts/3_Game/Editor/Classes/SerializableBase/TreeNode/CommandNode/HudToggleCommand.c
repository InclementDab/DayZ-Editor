class HudToggleCommand: CommandNode
{
	override bool SetState(TreeNodeState state)
	{
		if (!super.SetState(state)) {
			return false;
		}
		
		EditorHud hud = GetEditor().GetHud();
		if (!hud) {
			return false;
		}
		
		switch (state) {
			case TreeNodeState.EMPTY: {
				hud.Show(false);
				return true;
			}
			
			case TreeNodeState.ACTIVE: {
				hud.Show(true);
				return true;
			}
		}			
		
		return true;
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.TOGGLE;
	}
}