modded class WorldLighting
{
	protected int m_CurrentLighting;
	
	override void SetGlobalLighting(int lightingID)
	{
		super.SetGlobalLighting(lightingID);
		m_CurrentLighting = lightingID;
	}
	
	int GetCurrentLighting()
	{
		return m_CurrentLighting;
	}
	
	map<int, string> GetAllLightingConfigs()
	{
		map<int, string> lighting_configs = new map<int, string>();
		lighting_configs[0] = "Default";
		lighting_configs[1] = "Dark_Nights";
		
		// todo: no idea how the hell im gonna support other maps but ill figure it out
				
		return lighting_configs;
	}
}