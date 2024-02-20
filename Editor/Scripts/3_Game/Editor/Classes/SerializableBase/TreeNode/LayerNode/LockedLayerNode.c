class LockedLayerNode: LayerNode
{			
	override NodeState GetStateMask()
	{
		return NodeState.HOVER | NodeState.CONTEXT | NodeState.FOCUS | NodeState.ACTIVE | NodeState.EXTEND;
	}
}