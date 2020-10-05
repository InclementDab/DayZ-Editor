
class EditorEnvironmentDialogController: DialogBaseController
{
	protected World m_World;
	protected Weather m_Weather;
	
	float time, rain, fog, overcast, wind, exposure;
		
	void EditorEnvironmentDialogController()
	{
		m_World = GetGame().GetWorld();
		m_Weather = GetGame().GetWeather();
	}
	
	override void PropertyChanged(string property_name)
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
			
			case "exposure": {
				m_World.SetEyeAccom(exposure);
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
		
		float rain = GetGame().GetWeather().GetRain().GetActual();
		float fog = GetGame().GetWeather().GetFog().GetActual();
		float overcast = GetGame().GetWeather().GetOvercast().GetActual();
		
		GetGame().GetWeather().SetWindMaximumSpeed(100);
		float wind = GetGame().GetWeather().GetWindSpeed();
		
		float exposure = GetGame().GetWorld().GetEyeAccom();
		
		DropdownListPrefabItem list_prefab = new DropdownListPrefabItem("Test1");
		DropdownListPrefab dropdown = new DropdownListPrefab("This is a caption", m_Controller, "na", list_prefab);
		dropdown.InsertItem(list_prefab);
		dropdown.InsertItem("Test2");
		dropdown.InsertItem("Test3");
	
		GroupPrefab group_prefab = new GroupPrefab("Group1", m_Controller, string.Empty);
		
		
		group_prefab.Insert(dropdown);
		group_prefab.Insert(new SliderPrefab("Time", m_Controller, "time", time, 0, 86400));
		group_prefab.Insert(new SliderPrefab("Rain", m_Controller, "rain", rain, 0, 1));
		group_prefab.Insert(new SliderPrefab("Fog", m_Controller, "fog", fog, 0, 1));
		group_prefab.Insert(new SliderPrefab("Overcast", m_Controller, "overcast", overcast, 0, 1));
		group_prefab.Insert(new SliderPrefab("Wind", m_Controller, "wind", wind, 0, 1));
		group_prefab.Insert(new SliderPrefab("Exposure", m_Controller, "exposure", exposure, 0, 1));
				
		AddContent(group_prefab);
			

		AddButton(DialogResult.OK);
	}
	
	override typename GetControllerType() {
		return EditorEnvironmentDialogController;
	}
}