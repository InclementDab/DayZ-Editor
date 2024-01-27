class EditorHudController: ViewController
{	
	string Search;
	
	ref ObservableCollection<ref ScriptView> MenuItems = new ObservableCollection<ref ScriptView>(this);
	
	ref ObservableCollection<ref TreeView> LeftListItems = new ObservableCollection<ref TreeView>(this);
	ref ObservableCollection<ref TreeView> RightListItems = new ObservableCollection<ref TreeView>(this);
		
	ref ObservableCollection<ref EditorFileHeadView> OpenFiles = new ObservableCollection<ref EditorFileHeadView>(this);
		
	ref ObservableCollection<ref CompassTick> CompassTicks = new ObservableCollection<ref CompassTick>(this);
	
	ScrollWidget LeftScroll;
	ScrollWidget RightScroll;	
	
	// on this day, 1/23, DayZ Editor achieved beauty
}