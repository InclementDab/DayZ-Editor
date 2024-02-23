class EditorHudController: ViewController
{	
	string Search;
	
	ref ObservableCollection<ref NodeView> MenuItems = new ObservableCollection<ref NodeView>(this);
	
	ref ObservableCollection<ref NamedNodeView> LeftListItems = new ObservableCollection<ref NamedNodeView>(this);
	ref ObservableCollection<ref NamedNodeView> RightListItems = new ObservableCollection<ref NamedNodeView>(this);
		
	ref ObservableCollection<ref ObjectView> MapMarkers = new ObservableCollection<ref ObjectView>(this);
	
	ref ObservableCollection<ref EditorFileHeadView> OpenFiles = new ObservableCollection<ref EditorFileHeadView>(this);
		
	ScrollWidget LeftScroll;
	ScrollWidget RightScroll;	
	
	// on this day, 1/23, DayZ Editor achieved beauty
}