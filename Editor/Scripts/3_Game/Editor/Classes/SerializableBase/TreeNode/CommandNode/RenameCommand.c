class RenameCommand: NamedNode
{	
	override void OnInteract(NodeInteractType interact_type)
	{
		super.OnInteract(interact_type);
		
		if (interact_type & NodeInteractType.PRESS) {
			foreach (NodeView node_view: Views) {
				NamedNodeView named_node_view = NamedNodeView.Cast(node_view);
				if (named_node_view) {
					named_node_view.EnableRename();
				}
			}
		}
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.PRESS;
	}
}