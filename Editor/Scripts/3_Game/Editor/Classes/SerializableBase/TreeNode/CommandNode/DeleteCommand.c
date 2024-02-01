class DeleteCommand: CommandNode
{
	override void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
		super.OnStateChanged(state, total_state);
		
		if (total_state.IsActive()) {
			foreach (TreeNode node: TreeNode.StateMachine[TreeNodeState.ACTIVE]) {
				if (!node.CanDelete()) {
					continue;
				}
				
				GetEditor().InsertHistory(string.Format("Undo Delete %1", node.GetUUID()), Symbols.CLOCK_ROTATE_LEFT, null, node.CreateCopy());
				
				node.GetParent().Remove(node.GetUUID());
				node.GetParent().Synchronize();
				GetEditor().PlaySound(EditorSounds.HIGHLIGHT);
			}
		}
	}
}