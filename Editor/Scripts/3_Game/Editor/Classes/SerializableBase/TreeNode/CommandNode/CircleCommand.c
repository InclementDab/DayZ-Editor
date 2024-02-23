class EllipseCommand: NamedNode
{
	override void OnInteract(NodeInteractType interact_type, Widget widget = null)
	{
		super.OnInteract(interact_type, widget);
		
		if (interact_type & NodeInteractType.PRESS) {
			DaysBefore.GetEditor().GetHud().CurrentSelectionMode = SelectionMode.ELLIPSE;
		}
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.PRESS;
	}
}