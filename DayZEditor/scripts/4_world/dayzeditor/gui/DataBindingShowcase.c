
static ref DataBindingShowcaseController m_DataBindingShowcaseController;


class DataBindingShowcaseController: Controller
{
	
	protected string WindowText;
	int WindowSliderData;
	
	string WindowButton;
	
	void DataBindingShowcaseController()
	{
		m_DataBindingShowcaseController = this;
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("DataBindingShowcaseController::Init");
		
		super.OnWidgetScriptInit(w);
		
		
		WindowText = "REEE";
		NotifyPropertyChanged("WindowText");
		
		//WindowButton = "1";
		//NotifyPropertyChanged("WindowButton");
	}
	
	override void PropertyChanged(string property_name)
	{
		Print(property_name);
		Print(WindowButton);
	}
	
	
}

