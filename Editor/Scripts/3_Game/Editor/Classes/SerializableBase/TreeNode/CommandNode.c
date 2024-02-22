// I think its wise... it wasnt...
class CommandNode: NamedNode
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