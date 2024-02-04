class WeatherToggle: CommandNode
{
	protected int m_PreviousWeather = 0xFFFFFFFF;
	
	override void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
		super.OnStateChanged(state, total_state);
		
		if (state.IsActive()) {
			if (total_state.IsActive()) {
				GetEditor().Date = DateTime.Create(2007, 6, 1, 12);
				m_PreviousWeather = GetEditor().Climate;
				GetEditor().Climate = 0x00000000; // fixes climate change
			} else {
				GetEditor().Date = DateTime.Now(false);
				GetEditor().Climate = m_PreviousWeather;
			}
		}
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.TOGGLE;
	}
}