// I think its wise... it wasnt...
class CommandNode: SandboxNode
{
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.TOGGLE;
	}
	
	override NodeState GetStateMask()
	{
		return NodeState.ACTIVE | NodeState.HOVER;
	}
}