class BoxSelectCommand: TreeNode
{
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
				
		if (selected) {
			GetDayZGame().GetEditor().GetHud().CurrentSelectionMode = SelectionMode.BOX;
			GetDayZGame().GetEditor().GetNode("Commands").GetNode("CircleSelectCommand").SetSelected(false);
			GetDayZGame().GetEditor().GetNode("Commands").GetNode("LassoSelectCommand").SetSelected(false);
		}
	}
		
	override bool GetDefaultState()
	{
		return true;
	}
}
