class CreateFolder: NamedNode
{
	override void OnInteract(NodeInteractType interact_type)
	{
		super.OnInteract(interact_type);
		
		
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.PRESS;
	}
}