class WeatherToggle: CommandNode
{
	override void OnSelectionChanged(bool state)
	{
		GetEditor().WeatherDisable = state;
	}
}