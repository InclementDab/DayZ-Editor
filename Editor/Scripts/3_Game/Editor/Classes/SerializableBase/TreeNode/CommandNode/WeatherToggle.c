class WeatherToggle: CommandNode
{
	override void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
		super.OnStateChanged(state, total_state);
		
		if (state.IsActive()) {
			if (total_state.IsActive()) {
				GetGame().GetWorld().SetDate(2007, 6, 1, 12, 0);
			} else {
				GetDayZGame().SetDate(DateTime.Now());
			}
		}
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.TOGGLE;
	}
}