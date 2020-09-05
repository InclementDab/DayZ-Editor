



ref DataBindingShowcaseController m_DataBindingShowcaseController;

class DataBindingShowcaseController: Controller
{
	
	float WindowSlider = 25;
	bool EvaluateEquation;
	bool WindowTextToggle;
	int RadioButtons = 8;
	int SelectedObject;
	
	
	
	
	protected TextWidget WindowTextToggleValue;
	
	void DataBindingShowcaseController()
	{
		Print("DataBindingShowcaseController");
		m_DataBindingShowcaseController = this;	
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);

		

	}
	
	

	override void PropertyChanged(string property_name)
	{

		if (WindowSlider < 10) {
			WindowSlider = 10;
			NotifyPropertyChanged("WindowSlider");
		}
	}

}







