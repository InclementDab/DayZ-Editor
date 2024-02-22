class DeleteCommand: CommandNode
{
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
		
		if (node_state.IsActive() && state) {
			foreach (Node node: Node.States[NodeState.ACTIVE]) {
				if (!node) {
					continue;
				}
				
				DaysBefore.GetEditor().InsertHistory(null, node.CreateCopy());
				
				node.GetParent().Remove(node.GetUUID());
				node.GetParent().SetSynchDirty();
				DaysBefore.GetEditor().PlaySound(EditorSounds.HIGHLIGHT);
			}
		}
	}
}