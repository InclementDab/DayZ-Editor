


class DataBindingShowcaseController: Controller
{
	
	protected string WindowText;
	int WindowSliderData;
	
	bool WindowButton;

	
	override void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("DataBindingShowcaseController::Init");
		
		super.OnWidgetScriptInit(w);
		
		
		WindowText = "REEE";
		NotifyPropertyChanged("WindowText");
		
		WindowButton = true;
		NotifyPropertyChanged("WindowButton");
	}
	
}

