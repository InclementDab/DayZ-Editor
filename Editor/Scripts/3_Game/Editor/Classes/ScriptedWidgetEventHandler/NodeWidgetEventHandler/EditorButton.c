class EditorButton: NodeWidgetEventHandler
{
	override void OnStateChanged(TreeNode node, TreeNodeState state)
	{		
		if (!node.GetEditor().GetHud() || !Icon) {
			return;
		}
		
		if (state.IsHover()) {
			//WidgetAnimator.Animate(Icon, WidgetAnimatorProperty.COLOR_A, 1.0, 50);
			Icon.SetAlpha(1.0);
			node.GetEditor().GetHud().SetCursor(node.GetIcon(), node.GetDisplayName(), node.GetShortcutString());
		}
		
		if (state.IsActive()) {
			Icon.LoadImageFile(0, node.GetIcon().Solid());
		} else {
			node.GetEditor().GetHud().ClearCursor();
			Icon.LoadImageFile(0, node.GetIcon().Regular());
		}
		
		Icon.SetImage(0);
	}
}