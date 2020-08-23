


class DataBindingShowcaseController: Controller
{
	
	protected string WindowText;
	int WindowSliderData;
	
	string WindowButton;

	
	override void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("DataBindingShowcaseController::Init");
		
		super.OnWidgetScriptInit(w);
		
		
		WindowText = "REEE";
		NotifyPropertyChanged("WindowText");
		
		WindowButton = "1";
		NotifyPropertyChanged("WindowButton");
	}
	
}

