class AddLayerCommand: NamedNode
{	
	override void OnInteract(NodeInteractType interact_type, Widget widget = null)
	{
		super.OnInteract(interact_type, widget);
		
		if (interact_type & NodeInteractType.CLICK) {
			DaysBefore.GetEditor().GetPlacingDestination().Add(new LayerNode(UUID.Generate(), "layer0", Symbols.FOLDER, LinearColor.WHITE));
		}
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.CLICK;
	}
}