modded class MissionBase
{
	void MissionBase()
	{
		// Just bad vanilla code that doesnt enable this
		if (!GetGame().IsMultiplayer()) {
			m_WorldLighting	= new WorldLighting();
		}
	}
	
	override void OnMissionStart()
	{
		super.OnMissionStart();
		
		/* Random weather */
		GetGame().GetWeather().GetFog().Set(0.1, 0, 30);
		GetGame().GetWeather().GetOvercast().Set(Math.RandomFloat01() * 0.8, 0, 30);
		GetGame().GetWeather().GetRain().Set(Math.RandomFloat01() * 0.1, 0, 30);
		
		GetGame().GetWeather().SetWind(vector.Zero);
		GetGame().GetWeather().SetWindSpeed(0);
	}
}