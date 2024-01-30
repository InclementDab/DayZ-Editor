class CursorToggle: CommandNode
{
	override bool SetState(TreeNodeState state)
	{
		if (!super.SetState(state)) {
			return false;
		}
		
		if (GetEditor().GetHud()) {
			GetEditor().GetHud().ClearCursor();
		}
		
		switch (state) {
			case TreeNodeState.EMPTY: {
				GetGame().GetUIManager().ShowCursor(false);
				return true;
			}
			
			case TreeNodeState.ACTIVE: {
				GetGame().GetUIManager().ShowCursor(true);
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