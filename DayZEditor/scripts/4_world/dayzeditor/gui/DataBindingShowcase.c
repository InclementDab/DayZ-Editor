
ref DataBindingShowcaseController m_DataBindingShowcaseController;

class DataBindingShowcaseController: Controller
{
	
	float WindowSlider = 25;
	bool EvaluateEquation;
	bool WindowTextToggle;
	int RadioButtons = 8;
	int SelectedObject;
	
	ref ObservableCollection<string> test_collection = new ObservableCollection<string>("test_collection", this);
	
	protected TextWidget WindowTextToggleValue;
	
	string test;
	
	void DataBindingShowcaseController()
	{
		Print("DataBindingShowcaseController");
		m_DataBindingShowcaseController = this;	
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		test_collection.Insert("Test");
	}
	
	override void PropertyChanged(string property_name)
	{

		if (WindowSlider < 10) {
			WindowSlider = 10;
			NotifyPropertyChanged("WindowSlider");
		}
	}

}







