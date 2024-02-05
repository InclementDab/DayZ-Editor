class EditorButton: NodeWidgetEventHandler
{
	override void OnStateChanged(TreeNode node, TreeNodeState state)
	{		
		if (state.IsHover()) {
			if (node.GetState().IsHover()) {
				node.GetEditor().GetHud().SetCursor(node.GetIcon(), node.GetDisplayName(), node.GetShortcutString());
			} else {
				node.GetEditor().GetHud().ClearCursor();
			}
		}
		
		if (Icon) {
			int color = ARGB(100 + node.GetState().IsHover() * 155.0, 255, 255, 255);
			if (node.GetState().IsActive()) {
				color = m_LayoutRoot.GetColor();
				Icon.LoadImageFile(0, node.GetIcon().Solid());
			} else {
				Icon.LoadImageFile(0, node.GetIcon().Regular());
			}
			
			Icon.SetColor(color);		
			Icon.SetImage(0);
		}
	}
}