class EditorEnvironmentDialogController: DialogBaseController
{

}

class EditorEnvironment: SerializableBase
{
	float RainValue, FogValue, OvercastValue, SnowValue, WindDirValue, WindSpeedValue;
	float SnowScale;
	vector Wind;
	float DynamicFogDistanceDensity, DynamicFogHeightDensity, DynamicFogHeightBias;
	
	int Year, Month, Day, Hour, Minute;
	
	bool UseMissionWeather = false;
	
	void Capture(notnull Weather weather, notnull World world)
	{
		RainValue = weather.GetRain().GetActual();
		FogValue = weather.GetFog().GetActual();
		OvercastValue = weather.GetOvercast().GetActual();
		SnowValue = weather.GetSnowfall().GetActual();
		WindDirValue = weather.GetWindDirection().GetActual();
		WindSpeedValue = weather.GetWindDirection().GetActual();
		SnowScale = weather.GetSnowflakeScale();
		Wind = weather.GetWind();
		DynamicFogDistanceDensity = weather.GetDynVolFogDistanceDensity();
		DynamicFogHeightBias = weather.GetDynVolFogHeightBias();
		DynamicFogHeightDensity = weather.GetDynVolFogHeightDensity();
		
		world.GetDate(Year, Month, Day, Hour, Minute);
	}
	
	void Apply(notnull Weather weather, notnull World world)
	{
		weather.GetRain().Set(RainValue);
		weather.GetFog().Set(FogValue);
		weather.GetOvercast().Set(OvercastValue);
		weather.GetSnowfall().Set(SnowValue);
		weather.GetWindDirection().Set(WindDirValue);
		weather.GetWindDirection().Set(WindSpeedValue);
		weather.SetSnowflakeScale(SnowScale);
		weather.SetWind(Wind);
		weather.SetDynVolFogDistanceDensity(DynamicFogDistanceDensity);
		weather.SetDynVolFogHeightBias(DynamicFogHeightBias);
		weather.SetDynVolFogHeightDensity(DynamicFogHeightDensity);
		
		world.SetDate(Year, Month, Day, Hour, Minute);
	}

	override void Write(Serializer serializer, int version)
	{
		serializer.Write(RainValue);
		serializer.Write(FogValue);
		serializer.Write(OvercastValue);
		serializer.Write(SnowValue);
		serializer.Write(WindDirValue);
		serializer.Write(WindSpeedValue);
		serializer.Write(SnowScale);
		serializer.Write(Wind);
		serializer.Write(DynamicFogDistanceDensity);
		serializer.Write(DynamicFogHeightDensity);
		serializer.Write(DynamicFogHeightBias);
		serializer.Write(Year);
		serializer.Write(Month);
		serializer.Write(Day);
		serializer.Write(Hour);
		serializer.Write(Minute);
		serializer.Write(UseMissionWeather);
	}

	override bool Read(Serializer serializer, int version)
	{
		serializer.Read(RainValue);
		serializer.Read(FogValue);
		serializer.Read(OvercastValue);
		serializer.Read(SnowValue);
		serializer.Read(WindDirValue);
		serializer.Read(WindSpeedValue);
		serializer.Read(SnowScale);
		serializer.Read(Wind);
		serializer.Read(DynamicFogDistanceDensity);
		serializer.Read(DynamicFogHeightDensity);
		serializer.Read(DynamicFogHeightBias);
		serializer.Read(Year);
		serializer.Read(Month);
		serializer.Read(Day);
		serializer.Read(Hour);
		serializer.Read(Minute);
		serializer.Read(UseMissionWeather);

		return true;
	}

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
		GetEditor().UserEnvironment.Year = m_Year;
		GetEditor().UserEnvironment.Month = m_Month;
		GetEditor().UserEnvironment.Day = m_Day;
		GetEditor().UserEnvironment.Hour = m_Hour;
		GetEditor().UserEnvironment.Minute = m_Minute;
		
		GetEditor().UserEnvironment.SnowValue = snow;
		GetEditor().UserEnvironment.SnowScale = snowscale;
		
		GetEditor().UserEnvironment.RainValue = rain;
		GetEditor().UserEnvironment.FogValue = fog;
		GetEditor().UserEnvironment.OvercastValue = overcast;
		GetEditor().UserEnvironment.WindSpeedValue = wind;
		
		switch (property_name) {			
			case "m_LightingConfig": {
				m_WorldLighting.SetGlobalLighting(m_LightingConfig);
				break;
			}
		}
		
		if (GetGame().IsMultiplayer()) {
			ScriptRPC rpc = new ScriptRPC();
			GetEditor().UserEnvironment.Write(rpc, 0);
			rpc.Send(null, 39259, true);
		}
	}
}