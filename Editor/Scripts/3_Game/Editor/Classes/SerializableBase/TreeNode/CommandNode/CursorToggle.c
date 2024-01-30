class CursorToggle: CommandNode
{
	override void SetState(TreeNodeState state)
	{
		GetGame().GetUIManager().ShowCursor(state);
		if (GetEditor().GetHud()) {
			GetEditor().GetHud().ClearCursor();
		}
	}
}