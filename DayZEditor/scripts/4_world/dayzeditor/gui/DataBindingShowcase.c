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
	
	int RadioButtons = 8;
	
	ref ObservableCollection<ref EditorBrushData> WindowListWidgets;
	
	void DataBindingShowcaseController()
	{
		EditorLog.Trace("DataBindingShowcaseController");
		m_DataBindingShowcaseController = this;
	}
	

	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		WindowListWidgets = new ObservableCollection<ref EditorBrushData>("WindowListWidgets", this);
		
		ref EditorBrushData data = new EditorBrushData();
		data.Name = "Test";
		WindowListWidgets.Insert(data);
		
		ref EditorBrushData data2 = new EditorBrushData();
		data2.Name = "Test2";
		WindowListWidgets.Insert(data2);
	}
	
	override void PropertyChanged(string property_name)
	{
		
		if (property_name == "WindowTextToggleValue") {
			RadioButtons = 8;
			NotifyPropertyChanged("RadioButtons");
		}
	}
	
	// Return Value: Not implemented yet
	bool OnWindowTextToggle(ButtonCommandArgs args)
	{
		EditorLog.Trace("OnWindowTextToggle");
		
		return false;
	}
	
	bool OnWindowCheckToggle(CheckBoxCommandArgs args)
	{
		EditorLog.Trace("OnWindowCheckToggle");
		
		return false;
	}
	
	bool OnComboBoxChanged(XComboBoxCommandArgs args)
	{
		EditorLog.Trace("OnComboBoxChanged");
		return false;
	}
	
	bool CanComboBoxChange()
	{
		return true;
	}
	

}







