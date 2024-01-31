// I think its wise... it wasnt...
class CommandNode: TreeNode
{
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.PRESS;
	}
	
	override TreeNodeState GetStateMask()
	{
		return TreeNodeState.ACTIVE;
	}
}