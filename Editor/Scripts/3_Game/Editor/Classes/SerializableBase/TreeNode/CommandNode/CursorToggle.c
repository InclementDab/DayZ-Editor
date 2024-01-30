class CursorToggle: CommandNode
{
	override void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
		super.OnStateChanged(state, total_state);
		Print(state);
		if (GetEditor().GetHud()) {
			GetEditor().GetHud().ClearCursor();
		}
		
		GetGame().GetUIManager().ShowCursor(state.IsActive());
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.TOGGLE;
	}
}