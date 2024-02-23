class AddLayerCommand: NamedNode
{	
	override void OnInteract(NodeInteractType interact_type, Widget widget = null)
	{
		super.OnInteract(interact_type, widget);
		
		EditorNode editor = EditorNode.Cast(FindAncestor(EditorNode));
		if (interact_type & NodeInteractType.CLICK) {
			editor.GetLayers().Add(new LayerNode(UUID.Generate(), "layer0", Symbols.FOLDER, LinearColor.WHITE));
			editor.SetSynchDirty();
		}
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.CLICK;
	}
}