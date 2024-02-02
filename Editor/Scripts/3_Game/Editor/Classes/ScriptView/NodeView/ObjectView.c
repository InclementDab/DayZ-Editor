class ObjectView: NodeView
{
	protected ObjectNode m_ObjectNode;
	protected vector m_CursorAside = vector.Aside;
		
	ButtonWidget Button;
	ImageWidget Image, Outline;
	
	void ObjectView(TreeNode node)
	{
		m_ObjectNode = ObjectNode.Cast(node);		
		Symbols icon = m_ObjectNode.GetIcon();
		
		Image.LoadImageFile(0, icon.Regular());
		Image.SetImage(0);
		
		Outline.LoadImageFile(0, icon.Solid());
		Outline.SetImage(0);
	}
	
	override void OnStateChanged(TreeNode node, TreeNodeState state)
	{
		super.OnStateChanged(node, state);
		
		Outline.SetAlpha(state.IsHover());
				
		if (node.GetState().IsDragging() || node.GetState().IsActive()) {			
			Outline.SetColor(EditorColors.SELECT);
		} else {
			Outline.SetColor(ARGB(150, 255, 255, 255));
		}
		
		if (state.IsDragging()) {
			if (node.GetState().IsDragging()) {
				m_ObjectNode.GetEditor().GetHud().SetCursor(Symbols.UP_DOWN_LEFT_RIGHT);
			} else {
				m_ObjectNode.GetEditor().GetHud().ClearCursor();
			}
		}
	}
						
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Marker.layout";
	}
}