
class EditorEnvironmentDialogController: DialogBaseController
{
	protected World m_World;
	protected Weather m_Weather;
	
	float date, time, rain, fog, overcast, wind;
		
	void EditorEnvironmentDialogController()
	{
		m_World = GetGame().GetWorld();
		m_Weather = GetGame().GetWeather();
	}
	
	override void PropertyChanged(string property_name)
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
		}
	}
}

class EditorEnvironmentDialog: EditorDialogBase
{	
	void EditorEnvironmentDialog(string title)
	{
		//Debug_Logging = true;
		int year, month, day, hour, minute;
		GetGame().GetWorld().GetDate(year, month, day, hour, minute);
		float time = minute * 60 + hour * 3600;
		float date = month * 30 + day;
		
		float rain = GetGame().GetWeather().GetRain().GetActual();
		float fog = GetGame().GetWeather().GetFog().GetActual();
		float overcast = GetGame().GetWeather().GetOvercast().GetActual();
		
		GetGame().GetWeather().SetWindMaximumSpeed(100);
		float wind = GetGame().GetWeather().GetWindSpeed();
		
		float exposure = GetGame().GetWorld().GetEyeAccom();
		

		GroupPrefab group_prefab = new GroupPrefab("Weather", m_Controller, string.Empty);		
		group_prefab.Insert(new SliderPrefab("Date", m_Controller, "date", date, 1, 365));
		group_prefab.Insert(new SliderPrefab("Time", m_Controller, "time", time, 0, 86400));
		group_prefab.Insert(new SliderPrefab("Rain", m_Controller, "rain", rain, 0, 1));
		group_prefab.Insert(new SliderPrefab("Fog", m_Controller, "fog", fog, 0, 1));
		group_prefab.Insert(new SliderPrefab("Overcast", m_Controller, "overcast", overcast, 0, 1));
		group_prefab.Insert(new SliderPrefab("Wind", m_Controller, "wind", wind, 0, 1));
				
		AddContent(group_prefab);
			

		AddButton(DialogResult.OK);
	}
	
	override typename GetControllerType() {
		return EditorEnvironmentDialogController;
	}
}