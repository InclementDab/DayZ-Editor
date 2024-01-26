[RegisterCommand(LassoSelectCommand, "Lasso Selection", Symbols.LASSO)]
class LassoSelectCommand: EditorNode
{
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
				
		if (selected) {
			GetDayZGame().GetEditor().GetHud().CurrentSelectionMode = SelectionMode.LASSO;
			GetDayZGame().GetMaster().GetNode("SERVER").GetNode("CircleSelectCommand").SetSelected(false);
			GetDayZGame().GetMaster().GetNode("SERVER").GetNode("BoxSelectCommand").SetSelected(false);
		}
	}
}