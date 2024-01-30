class CursorToggle: CommandNode
{
	override void OnStateChanged(TreeNodeState state)
	{
		super.OnStateChanged(state);
		
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