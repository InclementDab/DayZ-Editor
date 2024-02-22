class MenuNode: NodeTreeView
{	
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
		
		if (node_state.IsHover()) {
			if (state) {
				WidgetAnimator.AnimateColor(Panel, EditorColor.SELECT, 1);
			} else {
				WidgetAnimator.AnimateColor(Panel, EditorColor.BACKGROUND_1, 100);
			}
		}	
	}
}