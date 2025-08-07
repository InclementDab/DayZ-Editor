class EditorEnvironmentDialogController: DialogBaseController
{

}

class EditorEnvironmentDialog: EditorDialogBase
{	
	protected World m_World;
	protected Weather m_Weather;
	protected WorldLighting m_WorldLighting;
	
	float rain, fog, overcast, wind, snow, snowscale, winddir;
	
	protected float m_DynFogDistanceDensity, m_DynFogHeightDensity, m_DynFogHeightBias;
	protected float m_Year, m_Month, m_Day, m_Hour, m_Minute;
	
	protected int m_LightingConfig;
	
	void EditorEnvironmentDialog(string title)
	{	
		m_World = GetGame().GetWorld();
		m_Weather = GetGame().GetWeather();
		m_WorldLighting = GetGame().GetMission().GetWorldLighting();
		
		m_LightingConfig = m_WorldLighting.GetCurrentLighting();
		
		int year, month, day, hour, minute;
		m_World.GetDate(year, month, day, hour, minute);
		m_Year = year;
		m_Month = month;
		m_Day = day;
		m_Hour = hour;
		m_Minute = minute;
		
		rain = m_Weather.GetRain().GetActual();
		fog = m_Weather.GetFog().GetActual();
		overcast = m_Weather.GetOvercast().GetActual();
		
		wind = m_Weather.GetWindMagnitude().GetActual();
		winddir = m_Weather.GetWindDirection().GetActual() * Math.RAD2DEG;
		
		m_DynFogHeightBias = m_Weather.GetDynVolFogHeightBias();
		m_DynFogHeightDensity = m_Weather.GetDynVolFogHeightDensity();
		m_DynFogDistanceDensity = m_Weather.GetDynVolFogDistanceDensity();
		
		GroupPrefab date_prefab = new GroupPrefab("Date / Time", this, string.Empty);
		date_prefab.Insert(new SliderPrefab("Year", this, "m_Year", 1970, 2038, 1));
		date_prefab.Insert(new SliderPrefab("Month", this, "m_Month", 1, 12, 1));
		date_prefab.Insert(new SliderPrefab("Day", this, "m_Day", 1, 31, 1));
		date_prefab.Insert(new SliderPrefab("Hour", this, "m_Hour", 1, 23, 1));
		date_prefab.Insert(new SliderPrefab("Minute", this, "m_Minute", 1, 59, 1));
		AddContent(date_prefab);
		
		GroupPrefab group_prefab = new GroupPrefab("#STR_EDITOR_WEATHER", this, string.Empty);
		group_prefab.Insert(new SliderPrefab("#STR_EDITOR_RAIN", this, "rain", 0, 1));
		group_prefab.Insert(new SliderPrefab("Snow", this, "snow", 0, 1));
		group_prefab.Insert(new SliderPrefab("Snow Scale", this, "snowscale", 0, 1));
		group_prefab.Insert(new SliderPrefab("#STR_EDITOR_FOG", this, "fog", 0, 1));
		group_prefab.Insert(new SliderPrefab("#STR_EDITOR_OVERCAST", this, "overcast", 0, 1));
		group_prefab.Insert(new SliderPrefab("Wind Direction", this, "winddir", 0, 360));
		group_prefab.Insert(new SliderPrefab("Wind Speed", this, "wind", 0, 100));
		group_prefab.Insert(new SliderPrefab("Dynamic Fog Distance Density", this, "m_DynFogDistanceDensity"));
		group_prefab.Insert(new SliderPrefab("Dynamic Fog Height Bias", this, "m_DynFogHeightBias"));
		group_prefab.Insert(new SliderPrefab("Dynamic Fog Height Density", this, "m_DynFogHeightDensity"));
		
		map<int, string> lighting_config_data = GetGame().GetMission().GetWorldLighting().GetAllLightingConfigs();
		DropdownListPrefab<int> lighting_config = new DropdownListPrefab<int>("#STR_EDITOR_LIGHTING_CONFIG", this, "m_LightingConfig");
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
			
	void PropertyChanged(string property_name)
	{		
		switch (property_name) {
			case "m_Year":
			case "m_Month":
			case "m_Day":
			case "m_Hour":
			case "m_Minute": {
				m_Year = Math.Round(m_Year);
				m_Month = Math.Round(m_Month);
				m_Day = Math.Round(m_Day);
				m_Hour = Math.Round(m_Hour);
				m_Minute = Math.Round(m_Minute);
				m_World.SetDate(m_Year, m_Month, m_Day, m_Hour, m_Minute);
				break;
			}
			
			case "snow":
			case "snowscale": {
				m_Weather.GetSnowfall().Set(snow);
				m_Weather.GetSnowfall().SetLimits(snow, snow);
				m_Weather.SetSnowflakeScale(snowscale);
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
			
			case "wind":
			case "winddir": {
				float winddir_rad = winddir * Math.DEG2RAD;
				m_Weather.GetWindDirection().Set(winddir_rad);
				m_Weather.GetWindDirection().SetLimits(winddir_rad, winddir_rad);
				m_Weather.GetWindDirection().SetForecastChangeLimits(winddir_rad, winddir_rad);
				m_Weather.GetWindMagnitude().Set(wind);
				m_Weather.GetWindMagnitude().SetLimits(wind, wind);
				m_Weather.GetWindMagnitude().SetForecastChangeLimits(wind, wind);
				break;
			}
			
			case "m_DynFogDistanceDensity": {
				m_Weather.SetDynVolFogDistanceDensity(m_DynFogDistanceDensity);
				break;
			}
			
			case "m_DynFogHeightBias": {
				m_Weather.SetDynVolFogHeightBias(m_DynFogHeightBias);
				break;
			}			
			
			case "m_DynFogHeightDensity": {
				m_Weather.SetDynVolFogHeightDensity(m_DynFogHeightDensity);
				break;
			}
			
			case "m_LightingConfig": {
				m_WorldLighting.SetGlobalLighting(m_LightingConfig);
				break;
			}
		}
		
		if (GetGame().IsMultiplayer()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(m_Year);
			rpc.Write(m_Month);
			rpc.Write(m_Day);
			rpc.Write(m_Hour);
			rpc.Write(m_Minute);
			rpc.Write(snow);
			rpc.Write(snowscale);
			rpc.Write(rain);
			rpc.Write(fog);
			rpc.Write(overcast);
			rpc.Write(winddir);
			rpc.Write(wind);
			rpc.Write(m_DynFogHeightBias);
			rpc.Write(m_DynFogHeightDensity);
			rpc.Write(m_DynFogDistanceDensity);
			rpc.Send(null, 39259, true);
		}
	}
}