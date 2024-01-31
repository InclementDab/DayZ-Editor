class MenuItemView: TreeView
{	
	override void OnStateChanged(TreeNode node, TreeNodeState state)
	{
		super.OnStateChanged(node, state);
		
		if (state.IsHover()) {
			if (node.GetState().IsHover()) {
				WidgetAnimator.AnimateColor(Panel, EditorColors.SELECT, 1);
			} else {
				WidgetAnimator.AnimateColor(Panel, EditorColors.BACKGROUND_1, 100);
			}
		}		
	}
}