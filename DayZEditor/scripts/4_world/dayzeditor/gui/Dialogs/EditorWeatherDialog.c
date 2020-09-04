
class EditorWeatherDialogController: Controller
{
	protected World m_World;
	protected Weather m_Weather;
		
	float time;
	float rain;
	float fog;
	float overcast;
	float wind;
	
	float view = 3000;
	float objectview = 1500;
	
	
	void SetWorld(World world)
	{
		EditorLog.Trace("EditorWeatherDialogController::SetWorld");
		m_World = world;
		m_Weather = GetGame().GetWeather();
		
		int year, month, day, hour, minute;
		m_World.GetDate(year, month, day, hour, minute);
		time = minute * 60 + hour * 3600;
		
		rain = m_Weather.GetRain();
		fog = m_Weather.GetFog();
		overcast = m_Weather.GetOvercast();
		
		m_Weather.SetWindMaximumSpeed(100);
		wind = m_Weather.GetWindSpeed();
	}
	
	override void NotifyPropertyChanged(string property_name)
	{
		
		switch (property_name) {
			
			case "time": {
				int year, month, day, hour, minute;
				m_World.GetDate(year, month, day, hour, minute);
				hour = Math.Floor(time / 3600); 
				minute = time / 60 - hour * 60;
				m_World.SetDate(year, month, day, hour, minute);
				
				break;
			}
			
			case "rain": {
				m_Weather.GetRain().(rain);				
				break;
			}
			
			case "fog": {
				m_Weather.GetFog().Set(fog);
				break;
			}
			
			case "overcast": {
				m_Weather.GetOvercast().Set(overcast);
				break;
			}
			
			case "wind": {
				m_Weather.SetWindSpeed(wind);
				break;
			}
			
			case "view": {
				m_World.SetViewDistance(view);
				break;
			}
			
			case "objectview": {
				m_World.SetObjectViewDistance(objectview);
				break;
			}
		}	
	}
}


class EditorWeatherDialog: EditorDialog
{
	protected ref EditorWeatherDialogController m_EditorWeatherDialogController;
	
	void EditorWeatherDialog()
	{
		EditorLog.Trace("EditorWeatherDialog");
		
		m_EditorWeatherDialogController = new EditorWeatherDialogController();		
		m_EditorWeatherDialogController.SetWorld(GetGame().GetWorld());
		
		EditorPrefabGroup weather_group = new EditorPrefabGroup("Weather");
		
		weather_group.AddPrefab(new EditorPrefabSlider("Time", "time", 0, 86400));
		weather_group.AddPrefab(new EditorPrefabSlider("Rain", "rain", 0, 1));
		weather_group.AddPrefab(new EditorPrefabSlider("Fog", "fog", 0, 1));
		weather_group.AddPrefab(new EditorPrefabSlider("Overcast", "overcast", 0, 1));
		weather_group.AddPrefab(new EditorPrefabSlider("Wind", "wind", 0, 100));
		weather_group.AddPrefab(new EditorPrefabSlider("View Distance", "view", 0, 20000));
		weather_group.AddPrefab(new EditorPrefabSlider("Object View Distance", "objectview", 0, 20000));
		
		weather_group.SetController(m_EditorWeatherDialogController);
		AddContent(weather_group);

		
		SetTitle("World Controller");
		AddButton("Lighting", "LightingCallback");
		AddButton("Close", "CloseCallback");
	}

	
	bool _LightingCallback = false;
	void LightingCallback()
	{
		_LightingCallback = !_LightingCallback;
		
		if (_LightingCallback) {
			GetGame().GetWorld().LoadNewLightingCfg( "dz\\data\\lighting\\lighting_darknight.txt" );
		} else {
			GetGame().GetWorld().LoadNewLightingCfg( "dz\\data\\lighting\\lighting_default.txt" );
		}
		
		
	}
	
	void CloseCallback()
	{
		CloseDialog();
	}	
	
}

