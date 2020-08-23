


class DataBindingShowcaseController: Controller
{
	
	protected bool WindowText;
	
	private float _WindowSliderData = 60;
	float WindowSliderData = _WindowSliderData;
	
	
	override void PropertyChanged(string property_name)
	{
		if (property_name == "WindowText") {
			
			if (WindowText)
				WindowSliderData = _WindowSliderData / 2;
			else WindowSliderData = _WindowSliderData;
			
			NotifyPropertyChanged("WindowSliderData");
		} 
		
		else if (property_name == "_WindowSliderData") {
			if (WindowText)
				WindowSliderData = _WindowSliderData / 2;
			else WindowSliderData = _WindowSliderData;
			
			NotifyPropertyChanged("WindowSliderData");
			
		}
	}

	
}

