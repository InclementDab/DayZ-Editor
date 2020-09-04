
class EditorWeatherDialogController: Controller
{
	protected World m_World;
	
	
	float time;
	float rain;
	
	
	void SetWorld(World world)
	{
		m_World = world;
		int year, month, day, hour, minute;
		m_World.GetDate(year, month, day, hour, minute);
		time = minute * 60 + hour * 3600;
	}
	
	override void NotifyPropertyChanged(string property_name)
	{
		
		switch (property_name) {
			
			case "time": {
				int year, month, day, hour, minute;
				m_World.GetDate(year, month, day, hour, minute);
				
				hour = time / 3600; 
				Print(hour);
				minute = time / 60 - hour * 60;
				Print(minute);
				m_World.SetDate(year, month, day, hour, minute);
				
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
		weather_group.AddPrefab(new EditorPrefabSlider("Rain", "rain"));
		
		weather_group.SetController(m_EditorWeatherDialogController);
		AddContent(weather_group);

		
		
		
		AddButton("Close", "CloseCallback");
	}

	
	void CloseCallback()
	{
		CloseDialog();
	}	
	
}

