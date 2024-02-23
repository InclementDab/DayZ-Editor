class LassoCommand: NamedNode
{
	override void OnInteract(NodeInteractType interact_type)
	{
		super.OnInteract(interact_type);
		
		if (interact_type & NodeInteractType.PRESS) {
			DaysBefore.GetEditor().GetHud().CurrentSelectionMode = SelectionMode.LASSO;
		}
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.PRESS;
	}
}