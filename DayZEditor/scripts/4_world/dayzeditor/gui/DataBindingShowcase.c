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
	
	ref ObservableCollection<Widget> WindowListWidgets;
	
	void DataBindingShowcaseController()
	{
		m_DataBindingShowcaseController = this;
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
	
		WindowListWidgets = new ObservableCollection<Widget>("WindowListWidgets", this);
		EditorListItem list_item = new EditorListItem();
		list_item.SetText("REEEEEE");
		WindowListWidgets.Insert(list_item.GetRoot());
		
		WindowText = true;
				
	}
	
	override void PropertyChanged(string property_name)
	{
		if (property_name == "WindowTextToggleValue") {
			WindowText = !WindowText;
			NotifyPropertyChanged("WindowText");
		}
		
		/*
		if (property_name == "EvaluateEquation") {
			WindowText.Evaluate();
			NotifyPropertyChanged("WindowText");
		}*/
	}
	

}





