class DeleteCommand: CommandNode
{
	override void OnStateChanged(TreeNodeState state)
	{
		super.OnStateChanged(state);
		
		if (m_TreeNodeState.IsActive()) {
			foreach (TreeNode node: TreeNode.StateMachine[TreeNodeState.ACTIVE]) {					
				GetEditor().InsertHistory("Undo Delete", Symbols.CLOCK_ROTATE_LEFT, null, node.CreateCopy());	
				node.GetParent().Children.Remove(node.GetUUID());				
				delete node;
				GetEditor().GetObjects().Synchronize();
				GetEditor().PlaySound(EditorSounds.HIGHLIGHT);
			}
		}
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.HOLD;
	}
}