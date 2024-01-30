class CursorToggle: CommandNode
{
	override void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
		super.OnStateChanged(state, total_state);
	
		if (GetEditor().GetHud()) {
			GetEditor().GetHud().ClearCursor();
		}
		
		GetGame().GetUIManager().ShowCursor(total_state.IsActive());
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.TOGGLE;
	}
}