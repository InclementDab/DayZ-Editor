class WeatherToggle: CommandNode
{
	override void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
		super.OnStateChanged(state, total_state);
		
		if (state.IsActive()) {
			if (total_state.IsActive()) {
				GetEditor().Date = DateTime.Create(2007, 6, 1, 12);
				
			} else {
				GetEditor().Date = DateTime.Now(false);
			}
		}
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.TOGGLE;
	}
}