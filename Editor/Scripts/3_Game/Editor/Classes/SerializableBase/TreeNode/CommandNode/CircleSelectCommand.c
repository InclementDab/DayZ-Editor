class CircleSelectCommand: TreeNode
{
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
				
		if (selected) {
			GetDayZGame().GetEditor().GetHud().CurrentSelectionMode = SelectionMode.ELLIPSE;
			GetDayZGame().GetEditor().GetNode("Commands").GetNode("BoxSelectCommand").SetSelected(false);
			GetDayZGame().GetEditor().GetNode("Commands").GetNode("LassoSelectCommand").SetSelected(false);
		}
	}
}