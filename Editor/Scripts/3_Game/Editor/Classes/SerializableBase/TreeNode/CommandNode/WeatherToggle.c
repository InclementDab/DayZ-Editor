class WeatherToggle: CommandNode
{
	protected int m_PreviousWeather = 0xFFFFFFFF;
	
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
		
		if (node_state.IsActive()) {
			if (state) {
				DaysBefore.GetEditor().Date = DateTime.Create(2007, 6, 1, 12);
				m_PreviousWeather = DaysBefore.GetEditor().Climate;
				DaysBefore.GetEditor().Climate = 0x00000000; // fixes climate change
			} else {
				DaysBefore.GetEditor().Date = DateTime.Now(false);
				DaysBefore.GetEditor().Climate = m_PreviousWeather;
			}
		}
	}
	
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.TOGGLE;
	}
}