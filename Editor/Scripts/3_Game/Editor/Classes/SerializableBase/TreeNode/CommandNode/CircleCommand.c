class EllipseCommand: NamedNode
{
	override void OnInteract(NodeInteractType interact_type, Widget widget = null)
	{
		super.OnInteract(interact_type, widget);
		
		EditorNode editor = EditorNode.Cast(FindAncestor(EditorNode));
		if (interact_type & NodeInteractType.PRESS) {
			editor.GetHud().CurrentSelectionMode = SelectionMode.ELLIPSE;
		}
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.PRESS;
	}
}