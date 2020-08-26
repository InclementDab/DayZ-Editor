//Print(__LINE__);
//Print(__FILE__); // useful shit // tools.pak?????

ref DataBindingShowcaseController m_DataBindingShowcaseController;

class DataBindingShowcaseController: Controller
{
	
	//EquationEvaluater WindowText = "(10 / 2) + 7";
	bool WindowText;
	float WindowSlider = 10;
	
	bool EvaluateEquation;
	
	bool WindowTextToggleValue;
	
	int RadioButtonGroup;
	
	ref ObservableCollection<Widget> WindowListWidgets;
	
	void DataBindingShowcaseController()
	{
		EditorLog.Trace("DataBindingShowcaseController");
		m_DataBindingShowcaseController = this;
	}
	

	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
	
		
		WindowText = true;
				
	}
	
	override void PropertyChanged(string property_name)
	{
		Print(RadioButtonGroup);
		
		if (property_name == "WindowTextToggleValue") {
			WindowText = !WindowText;
			NotifyPropertyChanged("WindowText");
		}
	}
	

}





