class EditorHudController: ViewController
{	
	string Search;
	
	ref ObservableCollection<EditorNodeView> LeftListItems = new ObservableCollection<EditorNodeView>(this);
	ref ObservableCollection<EditorNodeView> RightListItems = new ObservableCollection<EditorNodeView>(this);
		
	ref ObservableCollection<ref EditorFileHeadView> OpenFiles = new ObservableCollection<ref EditorFileHeadView>(this);
		
	ref ObservableCollection<ref CompassTick> CompassTicks = new ObservableCollection<ref CompassTick>(this);
	
	ScrollWidget LeftScroll;
	ScrollWidget RightScroll;	
	
	// on this day, 1/23, DayZ Editor achieved beauty
}