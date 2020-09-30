
class EditorEnvironmentDialogController: DialogBaseController
{
	
	float TimeValue;
		
	override void PropertyChanged(string property_name)
	{
	
	}
}

class EditorEnvironmentDialog: EditorDialogBase
{
	ref SliderPrefab TimeSlider;
	
	void EditorEnvironmentDialog(Widget parent = null, string title = "")
	{
		TimeSlider = new SliderPrefab(null, "Time", m_Controller, "TimeValue");
		AddContent(TimeSlider);

		AddButton(DialogResult.OK);
	}
	
	override typename GetControllerType() {
		return EditorEnvironmentDialogController;
	}
}