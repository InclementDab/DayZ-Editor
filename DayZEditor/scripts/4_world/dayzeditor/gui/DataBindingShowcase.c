


class DataBindingShowcaseController: Controller
{
	
	protected DataBinding<string> WindowText;
	DataBinding<float> WindowSliderData;

	
	override void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("DataBindingShowcaseController::Init");
		
		super.OnWidgetScriptInit(w);
	}
	
}

