class BlowoutEventSettings
{
	// 0..1 value
	float Intensity = 0.5;
	
	// Amount of waves
	int WaveCount = 3;
	
	// Time between waves in seconds
	float TimeBetweenWaves = 5;
	
	// Time between StartBlowout and actual event in seconds
	float BlowoutDelay = 15; // 120
	
	float ImpactShockDamage = 15;
	
	float BlowoutSize = 10000;
	
	float BlowoutCount = 3;
	
	static ref array<vector> GetAlarmPositions()
	{
		
		ref array<vector> alarm_positions = {};
		string world_name;
		GetGame().GetWorldName(world_name);
		string cfg = "CfgWorlds " + world_name + " Names";		
		
		string allowed_types = "Capital City";
		
		for (int i = 0; i < GetGame().ConfigGetChildrenCount(cfg); i++) {
			string city;
			GetGame().ConfigGetChildName(cfg, i, city);			
			vector city_position = GetGame().ConfigGetVector(string.Format("%1 %2 position", cfg, city));
			if (allowed_types.Contains(GetGame().ConfigGetTextOut(string.Format("%1 %2 type", cfg, city)))) {
				alarm_positions.Insert(city_position);
			}
		}
		
		return alarm_positions;
	}
}