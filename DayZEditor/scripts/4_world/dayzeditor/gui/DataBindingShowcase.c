


class DataBindingShowcaseController: Controller
{
	
	protected string WindowText = "true";
	private float WindowSliderData = 60;
	
	bool WindowTextToggleValue;
	
	override void PropertyChanged(string property_name)
	{
		if (property_name == "WindowTextToggleValue") {
			WindowSliderData = 100 * WindowTextToggleValue;
			NotifyPropertyChanged("WindowSliderData");
		}
	}
}

