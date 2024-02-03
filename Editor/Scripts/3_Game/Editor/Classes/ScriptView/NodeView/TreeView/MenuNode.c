class MenuNode: TreeView
{	
	override void OnStateChanged(TreeNode node, TreeNodeState state)
	{
		super.OnStateChanged(node, state);
		
		if (state.IsHover()) {
			if (node.GetState().IsHover()) {
				WidgetAnimator.AnimateColor(Panel, EditorColor.SELECT, 1);
			} else {
				WidgetAnimator.AnimateColor(Panel, EditorColor.BACKGROUND_1, 100);
			}
		}	
	}
}