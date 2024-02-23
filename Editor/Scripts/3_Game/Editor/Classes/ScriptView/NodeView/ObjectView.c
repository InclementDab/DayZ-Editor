class ObjectView: NodeView
{
	protected ObjectNode m_ObjectNode;
	protected vector m_CursorAside = vector.Aside;
		
	ButtonWidget Button;
	ImageWidget Image, Outline;
	
	void ObjectView(Node node)
	{
		m_ObjectNode = ObjectNode.Cast(node);		
		Symbols icon = m_ObjectNode.Icon;
		
		Image.LoadImageFile(0, icon.Regular());
		Image.SetImage(0);
		
		Outline.LoadImageFile(0, icon.Solid());
		Outline.SetImage(0);
	}
	
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
		
		if (node_state.IsHover()) {
			Outline.SetAlpha(state);
		}
				
		if (node_state.IsDrag() || state) {
			if (state) {
				Outline.SetColor(EditorColor.SELECT);
			} else {
				Outline.SetColor(ARGB(150, 255, 255, 255));
			}
		}
		
		if (node_state.IsDrag()) {
			if (state) {
				GetDayZGame().SetCursor(Symbols.UP_DOWN_LEFT_RIGHT);
			} else {
				GetDayZGame().ClearCursor();
			}
		}
	}
						
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Marker.layout";
	}
}