[RegisterCommand(BoxSelectCommand, "Box Selection", Symbols.SQUARE_DASHED)]
class BoxSelectCommand: EditorNode
{
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
				
		if (selected) {
			GetDayZGame().GetEditor().GetHud().CurrentSelectionMode = SelectionMode.BOX;
			GetDayZGame().GetMaster().GetNode("Commands").GetNode("CircleSelectCommand").SetSelected(false);
			GetDayZGame().GetMaster().GetNode("Commands").GetNode("LassoSelectCommand").SetSelected(false);
		}
	}
		
	override bool GetDefaultState()
	{
		return true;
	}
}
