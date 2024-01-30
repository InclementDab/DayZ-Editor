class EditorButton: NodeWidgetEventHandler
{
	override void OnStateChanged(TreeNode node, TreeNodeState state)
	{		
		if (!node.GetEditor().GetHud() || !Icon) {
			return;
		}
		
		if (state.IsHover()) {
			node.GetEditor().GetHud().SetCursor(node.GetIcon(), node.GetDisplayName(), node.GetShortcutString());
		} else {
			node.GetEditor().GetHud().ClearCursor();
		}
		
		int color = ARGB(100 + state.IsHover() * 155.0, 255, 255, 255);
		if (state.IsActive()) {
			color = m_LayoutRoot.GetColor();
			Icon.LoadImageFile(0, node.GetIcon().Solid());
		} else {
			Icon.LoadImageFile(0, node.GetIcon().Regular());
		}
		
		Icon.SetColor(color);		
		Icon.SetImage(0);
	}
}