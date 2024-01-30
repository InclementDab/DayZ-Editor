class HudToggleCommand: CommandNode
{
	override bool SetState(TreeNodeState state)
	{
		if (!super.SetState(state)) {
			return false;
		}
		
		if (GetEditor().GetHud()) {
			GetEditor().GetHud().Show(state);
		}
		
		return true;
	}
}