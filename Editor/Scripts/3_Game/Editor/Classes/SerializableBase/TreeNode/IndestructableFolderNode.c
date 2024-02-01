class IndestructableFolderNode: FolderNode
{
	override bool CanDelete()
	{
		return false;
	}
	
	override TreeNodeState GetStateMask()
	{
		return TreeNodeState.HOVER | TreeNodeState.CONTEXT | TreeNodeState.FOCUS;
	}
}