class LockedLayerNode: LayerNode
{			
	override NodeState GetStateMask()
	{
		return NodeState.ACTIVE | NodeState.EXTEND;
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.ENTER | NodeInteractType.LEAVE | NodeInteractType.CONTEXT;
	}
}