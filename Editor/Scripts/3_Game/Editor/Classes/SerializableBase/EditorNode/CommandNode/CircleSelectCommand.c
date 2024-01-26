class CircleSelectCommand: EditorNode
{
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
				
		if (selected) {
			GetDayZGame().GetEditor().GetHud().CurrentSelectionMode = SelectionMode.ELLIPSE;
			GetDayZGame().GetMaster().GetNode("Commands").GetNode("BoxSelectCommand").SetSelected(false);
			GetDayZGame().GetMaster().GetNode("Commands").GetNode("LassoSelectCommand").SetSelected(false);
		}
	}
}