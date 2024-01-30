class DeleteCommand: CommandNode
{
	override void SetState(TreeNodeState state)
	{
		/*
		array<TreeNode> selected_nodes = GetEditor().GetSelectedNodes();
		foreach (TreeNode node: selected_nodes) {					
			GetEditor().InsertHistory("Undo Delete", Symbols.CLOCK_ROTATE_LEFT, null, node.CreateCopy());	
			node.GetParent().Children.Remove(node.GetUUID());				
			delete node;
			GetEditor().GetObjects().Synchronize();
			GetEditor().PlaySound(EditorSounds.HIGHLIGHT);
		}*/
	}
}