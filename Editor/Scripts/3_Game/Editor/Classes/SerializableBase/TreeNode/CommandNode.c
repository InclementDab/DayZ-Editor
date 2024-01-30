// I think its wise... it wasnt...
class CommandNode: TreeNode
{			
	override TreeNodeState GetStateMask()
	{
		return TreeNodeState.ACTIVE;
	}
}