// I think its wise... it wasnt...
class CommandNode: TreeNode
{
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.TOGGLE;
	}
	
	override TreeNodeState GetStateMask()
	{
		return TreeNodeState.ACTIVE | TreeNodeState.HOVER;
	}
}