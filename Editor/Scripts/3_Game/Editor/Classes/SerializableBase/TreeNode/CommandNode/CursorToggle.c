class CursorToggle: CommandNode
{
	override bool SetState(TreeNodeState state)
	{
		if (!super.SetState(state)) {
			return false;
		}
		
		GetGame().GetUIManager().ShowCursor(state);
		if (GetEditor().GetHud()) {
			GetEditor().GetHud().ClearCursor();
		}
		
		return true;
	}
}