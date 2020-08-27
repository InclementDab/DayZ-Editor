//Print(__LINE__);
//Print(__FILE__); // useful shit // tools.pak?????

ref DataBindingShowcaseController m_DataBindingShowcaseController;

class DataBindingShowcaseController: Controller
{
	
	EquationEvaluater WindowText;
	//bool WindowText;
	float WindowSlider = 10;
	
	bool EvaluateEquation;
	
	bool WindowTextToggle;
	
	int RadioButtons = 8;
	
	int SelectedObject;
	
	ref ObservableCollection<ref EditorBrushData> WindowListWidgets;
	
	TextWidget WindowTextToggleValue;
	
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
		
	}
	

	void OnWindowTextToggle(ButtonCommandArgs args)
	{
		EditorLog.Trace("OnWindowTextToggle");
		
		int tick = TickCount(0);
		WindowText = "(10 / 2) + 7";
		NotifyPropertyChanged("WindowText");
		Print(TickCount(tick));
		
		
		tick = TickCount(0);
		WindowText = WindowText.Evaluate().ToString();
		NotifyPropertyChanged("WindowText");
		Print(TickCount(tick));
	}
	
	void OnWindowCheckToggle(CheckBoxCommandArgs args)
	{
		EditorLog.Trace("OnWindowCheckToggle");
		
		
	}
	
	void OnComboBoxChanged(XComboBoxCommandArgs args)
	{
		EditorLog.Trace("OnComboBoxChanged");

	}
	
	bool CanComboBoxChange()
	{
		return true;
	}
	

}







