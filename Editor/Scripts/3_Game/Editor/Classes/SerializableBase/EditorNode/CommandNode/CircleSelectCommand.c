[RegisterCommand(CircleSelectCommand, "Circle Selection", Symbols.CIRCLE_DASHED)]
class CircleSelectCommand: EditorNode
{
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
				
		if (selected) {
			GetDayZGame().GetEditor().GetHud().CurrentSelectionMode = SelectionMode.ELLIPSE;
			GetDayZGame().GetMaster().GetNode("SERVER").GetNode("BoxSelectCommand").SetSelected(false);
			GetDayZGame().GetMaster().GetNode("SERVER").GetNode("LassoSelectCommand").SetSelected(false);
		}
	}
}