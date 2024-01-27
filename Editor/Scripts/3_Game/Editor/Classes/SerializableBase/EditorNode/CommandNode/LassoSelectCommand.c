class LassoSelectCommand: EditorNode
{
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
				
		if (selected) {
			GetDayZGame().GetEditor().GetHud().CurrentSelectionMode = SelectionMode.LASSO;
			GetDayZGame().GetEditor().GetNode("Commands").GetNode("CircleSelectCommand").SetSelected(false);
			GetDayZGame().GetEditor().GetNode("Commands").GetNode("BoxSelectCommand").SetSelected(false);
		}
	}
}