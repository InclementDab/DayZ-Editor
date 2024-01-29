class HudToggleCommand: CommandNode
{
	override void OnSelectionChanged(bool state)
	{
		super.OnSelectionChanged(state);
		
		if (GetEditor().GetHud()) {
			GetEditor().GetHud().Show(state);
		}
	}
}