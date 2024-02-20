class DeleteCommand: CommandNode
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(state, total_state);
		
		if (total_state.IsActive()) {
			foreach (TreeNode node: TreeNode.StateMachine[NodeState.ACTIVE]) {
				if (!node || !node.CanDelete()) {
					continue;
				}
				
				GetEditor().InsertHistory(string.Format("Undo Delete %1", node.GetUUID()), Symbols.CLOCK_ROTATE_LEFT, null, node.CreateCopy());
				
				node.Parent.Remove(node.GetUUID());
				node.Parent.Synchronize();
				GetEditor().PlaySound(EditorSounds.HIGHLIGHT);
			}
		}
	}
}