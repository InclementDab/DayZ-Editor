class EditorEnvironmentDialogController: DialogBaseController
{

}

class EditorEnvironmentDialog: EditorDialogBase
{	
	void EditorEnvironmentDialog(string title)
	{	
		m_World = GetGame().GetWorld();
		m_Weather = GetGame().GetWeather();
		m_WorldLighting = GetGame().GetMission().GetWorldLighting();
		
		LightingConfig = m_WorldLighting.GetCurrentLighting();
		
		int year, month, day, hour, minute;
		GetGame().GetWorld().GetDate(year, month, day, hour, minute);
		time = minute * 60 + hour * 3600;
		date = month * 30 + day;
		
		rain = GetGame().GetWeather().GetRain().GetActual();
		fog = GetGame().GetWeather().GetFog().GetActual();
		overcast = GetGame().GetWeather().GetOvercast().GetActual();
		
		wind = GetGame().GetWeather().GetWindSpeed();
		
		GroupPrefab group_prefab = new GroupPrefab("#STR_EDITOR_WEATHER", this, string.Empty);		
		group_prefab.Insert(new SliderPrefab("#STR_EDITOR_DATE", this, "date", 1, 365));
		group_prefab.Insert(new SliderPrefab("#STR_EDITOR_TIME", this, "time", 0, 86400));
		group_prefab.Insert(new SliderPrefab("#STR_EDITOR_RAIN", this, "rain", 0, 1));
		group_prefab.Insert(new SliderPrefab("#STR_EDITOR_FOG", this, "fog", 0, 1));
		group_prefab.Insert(new SliderPrefab("#STR_EDITOR_OVERCAST", this, "overcast", 0, 1));
		group_prefab.Insert(new SliderPrefab("#STR_EDITOR_WIND", this, "wind", 0, 1));
		
		map<int, string> lighting_config_data = GetGame().GetMission().GetWorldLighting().GetAllLightingConfigs();
		DropdownListPrefab<int> lighting_config = new DropdownListPrefab<int>("#STR_EDITOR_LIGHTING_CONFIG", this, "LightingConfig");
		foreach (int value, string name: lighting_config_data) {
			lighting_config[name] = value;
		}
		
		group_prefab.Insert(lighting_config);
		
		AddContent(group_prefab);
		AddButton(DialogResult.OK);
	}
	
	override typename GetControllerType() 
	{
		return EditorEnvironmentDialogController;
	}
	
	override string GetIcon() 
	{
		return "set:dayz_gui image:icon_moon";
	}
	
	protected World m_World;
	protected Weather m_Weather;
	protected WorldLighting m_WorldLighting;
	
	float date, time, rain, fog, overcast, wind;
		
	int LightingConfig;
		
	void PropertyChanged(string property_name)
	{
		switch (property_name) {
						
			case "date":
			case "time": {
				int year, month, day, hour, minute;
				m_World.GetDate(year, month, day, hour, minute);
				hour = Math.Floor(time / 3600); 
				minute = time / 60 - hour * 60;
				month = Math.Floor(date / 30);
				day = date - month * 30;
				m_World.SetDate(year, month, day, hour, minute);
				break;
			}
			
			case "rain": {
				m_Weather.GetRain().Set(rain);	
				m_Weather.GetRain().SetLimits(rain, rain);			
				break;
			}
			
			case "fog": {
				m_Weather.GetFog().Set(fog);
				m_Weather.GetFog().SetLimits(fog, fog);
				break;
			}
			
			case "overcast": {
				m_Weather.GetOvercast().Set(overcast);
				m_Weather.GetOvercast().SetLimits(overcast, overcast);
				break;
			}
			
			case "wind": {
				//m_Weather.SetWindFunctionParams(wind, wind, wind);
				m_Weather.SetWindSpeed(wind);
				m_Weather.SetWindMaximumSpeed(wind);
				break;
			}
			
			case "LightingConfig": {
				m_WorldLighting.SetGlobalLighting(LightingConfig);
				break;
			}
		}
	}
}