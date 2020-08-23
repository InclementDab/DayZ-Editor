
static ref DataBindingShowcaseController m_DataBindingShowcaseController;


class DataBindingShowcaseController: Controller
{
	
	protected string WindowText;
	float WindowSliderData = 50;
	string WindowButton;
	
	void DataBindingShowcaseController()
	{
		m_DataBindingShowcaseController = this;
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("DataBindingShowcaseController::Init");
		
		super.OnWidgetScriptInit(w);
		
	}
	
	override void PropertyChanged(string property_name)
	{
		Print(WindowSliderData);
	}
	
	
}

