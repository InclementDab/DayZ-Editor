class LockedLayerNode: LayerNode
{	
	override bool CanDelete()
	{
		return false;
	}
		
	override TreeNodeState GetStateMask()
	{
		return TreeNodeState.HOVER | TreeNodeState.CONTEXT | TreeNodeState.FOCUS | TreeNodeState.ACTIVE | TreeNodeState.EXTEND;
	}
}