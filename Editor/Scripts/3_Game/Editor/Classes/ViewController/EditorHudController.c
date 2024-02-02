class EditorHudController: ViewController
{	
	string Search;
	
	ref ObservableCollection<ref MenuNode> MenuItems = new ObservableCollection<ref MenuNode>(this);
	
	ref ObservableCollection<ref TreeView> LeftListItems = new ObservableCollection<ref TreeView>(this);
	ref ObservableCollection<ref TreeView> RightListItems = new ObservableCollection<ref TreeView>(this);
		
	ref ObservableCollection<ref EditorFileHeadView> OpenFiles = new ObservableCollection<ref EditorFileHeadView>(this);
		
	ScrollWidget LeftScroll;
	ScrollWidget RightScroll;	
	
	// on this day, 1/23, DayZ Editor achieved beauty
}