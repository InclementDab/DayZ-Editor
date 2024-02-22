class LockedLayerNode: LayerNode
{			
	override NodeState GetStateMask()
	{
		return NodeState.HOVER | NodeState.CONTEXT | NodeState.ACTIVE | NodeState.EXTEND;
	}
}