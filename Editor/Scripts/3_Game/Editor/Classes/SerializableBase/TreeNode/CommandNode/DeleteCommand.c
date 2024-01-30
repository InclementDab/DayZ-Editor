class DeleteCommand: CommandNode
{
	override bool SetState(TreeNodeState state)
	{
		if (!super.SetState(state)) {
			return false;
		}
		
		switch (state) {
			case TreeNodeState.ACTIVE: {
				foreach (TreeNode node: TreeNode.StateMachine[TreeNodeState.ACTIVE]) {					
					GetEditor().InsertHistory("Undo Delete", Symbols.CLOCK_ROTATE_LEFT, null, node.CreateCopy());	
					node.GetParent().Children.Remove(node.GetUUID());				
					delete node;
					GetEditor().GetObjects().Synchronize();
					GetEditor().PlaySound(EditorSounds.HIGHLIGHT);
				}
				
				return true;
			}
		}
		
		return true;
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.HOLD;
	}
}