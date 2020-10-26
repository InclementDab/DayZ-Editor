
class EditorEnvironmentDialogController: DialogBaseController
{
	protected World m_World;
	protected Weather m_Weather;
	
	float date, time, rain, fog, overcast, wind;
	int year, month, day, hour, minute;
		
	void EditorEnvironmentDialogController()
	{
		m_World = GetGame().GetWorld();
		m_Weather = GetGame().GetWeather();
		

		GetGame().GetWorld().GetDate(year, month, day, hour, minute);
		time = minute * 60 + hour * 3600;
		date = month * 30 + day;
		
		rain = GetGame().GetWeather().GetRain().GetActual();
		fog = GetGame().GetWeather().GetFog().GetActual();
		overcast = GetGame().GetWeather().GetOvercast().GetActual();
		
		wind = GetGame().GetWeather().GetWindSpeed();
	}
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
						
			case "date":
			case "time": {
				
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
		}
	}
}

class EditorEnvironmentDialog: EditorDialogBase
{	
	void EditorEnvironmentDialog(string title)
	{
		//Debug_Logging = true;

		

		GroupPrefab group_prefab = new GroupPrefab("Weather", m_Controller, string.Empty);		
		group_prefab.Insert(new SliderPrefab("Date", m_Controller, "date", 1, 365));
		group_prefab.Insert(new SliderPrefab("Time", m_Controller, "time", 0, 86400));
		group_prefab.Insert(new SliderPrefab("Rain", m_Controller, "rain", 0, 1));
		group_prefab.Insert(new SliderPrefab("Fog", m_Controller, "fog", 0, 1));
		group_prefab.Insert(new SliderPrefab("Overcast", m_Controller, "overcast", 0, 1));
		group_prefab.Insert(new SliderPrefab("Wind", m_Controller, "wind", 0, 1));
				
		AddContent(group_prefab);
			

		AddButton(DialogResult.OK);
	}
	
	override typename GetControllerType() {
		return EditorEnvironmentDialogController;
	}
}