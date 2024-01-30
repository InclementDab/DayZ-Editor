class AfterlifeToggle: CommandNode
{
	override bool SetState(TreeNodeState state)
	{
		
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.TOGGLE;
	}
}