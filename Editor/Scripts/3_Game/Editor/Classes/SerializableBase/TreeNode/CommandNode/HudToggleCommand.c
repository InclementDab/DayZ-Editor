class HudToggleCommand: CommandNode
{
	override void SetState(TreeNodeState state)
	{
		super.SetState(state);
		
		if (GetEditor().GetHud()) {
			GetEditor().GetHud().Show(state);
		}
	}
}