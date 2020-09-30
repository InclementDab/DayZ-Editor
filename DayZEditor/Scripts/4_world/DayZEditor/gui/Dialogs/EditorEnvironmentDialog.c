
class EditorEnvironmentDialogController: DialogBaseController
{
	protected World m_World;
	
	float Time;
		
	void EditorEnvironmentDialogController()
	{
		m_World = GetGame().GetWorld();
		
		int year, month, day, hour, minute;
		m_World.GetDate(year, month, day, hour, minute);
		Time = minute * 60 + hour * 3600;
	}
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			
			case "Time": {
				int year, month, day, hour, minute;
				m_World.GetDate(year, month, day, hour, minute);
				hour = Math.Floor(Time / 3600); 
				minute = Time / 60 - hour * 60;
				m_World.SetDate(year, month, day, hour, minute);
				break;
			}
			
		}
	}
}

class EditorEnvironmentDialog: EditorDialogBase
{	
	void EditorEnvironmentDialog(Widget parent = null, string title = "")
	{
		AddContent(new SliderPrefab(null, "Time", m_Controller, "Time", 0, 86400));

		AddButton(DialogResult.OK);
	}
	
	override typename GetControllerType() {
		return EditorEnvironmentDialogController;
	}
}