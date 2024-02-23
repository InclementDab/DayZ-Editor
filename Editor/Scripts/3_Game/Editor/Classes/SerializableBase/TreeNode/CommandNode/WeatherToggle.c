class WeatherToggle: NamedNode
{
	override void OnInteract(NodeInteractType interact_type)
	{
		super.OnInteract(interact_type);
		
		ToggleState(NodeState.ACTIVE);
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.PRESS;
	}
}