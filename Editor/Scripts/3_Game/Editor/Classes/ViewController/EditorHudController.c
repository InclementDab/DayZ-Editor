class EditorHudController: ViewController
{	
	string Search;
	
	ref ObservableCollection<ref ScriptView> MenuItems = new ObservableCollection<ref ScriptView>(this);
	
	ref ObservableCollection<ref EditorNodeView> LeftListItems = new ObservableCollection<ref EditorNodeView>(this);
	ref ObservableCollection<ref EditorNodeView> RightListItems = new ObservableCollection<ref EditorNodeView>(this);
		
	ref ObservableCollection<ref EditorFileHeadView> OpenFiles = new ObservableCollection<ref EditorFileHeadView>(this);
		
	ref ObservableCollection<ref CompassTick> CompassTicks = new ObservableCollection<ref CompassTick>(this);
	
	ScrollWidget LeftScroll;
	ScrollWidget RightScroll;	
	
	// on this day, 1/23, DayZ Editor achieved beauty
}