class LayerNode: NamedNode
{	
	override array<Node> GetContextMenu()
	{
		return { 
			DaysBefore.GetEditor().GetCommand("Cut"),
			DaysBefore.GetEditor().GetCommand("Copy"),
			DaysBefore.GetEditor().GetCommand("Paste"),
			DaysBefore.GetEditor().GetCommand("AddLayer"),
			DaysBefore.GetEditor().GetCommand("SetLayerActive"),
		};
	}
			
	override NodeState GetStateMask()
	{
		return NodeState.ACTIVE | NodeState.EXTEND;
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.ENTER | NodeInteractType.LEAVE | NodeInteractType.CONTEXT | NodeInteractType.DRAG_START | NodeInteractType.DRAG | NodeInteractType.DROP | NodeInteractType.CLICK;
	}
	
	override void OnInteract(NodeInteractType interact_type)
	{
		super.OnInteract(interact_type);
		
		if (interact_type & NodeInteractType.CLICK) {
			foreach (Node child: Children) {
				child.AddState(NodeState.ACTIVE);
			}
		}
	}
}