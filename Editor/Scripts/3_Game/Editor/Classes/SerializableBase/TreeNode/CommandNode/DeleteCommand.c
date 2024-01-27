class DeleteCommand: TreeNode
{
	override void SetSelected(bool state) 
	{
		Print("Cringe"); 
		foreach (TreeNode node: TreeNode.SelectedObjects) {
			Print(node);	
			delete node;
		}
	}
}