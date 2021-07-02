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
	
	map<string, int> GetAllLightingConfigs()
	{
		map<string, int> lighting_configs = new map<string, int>();
		lighting_configs["Default"] = 0;
		lighting_configs["Dark_Nights"] = 1;
		return lighting_configs;
	}
}