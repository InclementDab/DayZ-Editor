class EditorHudController: ViewController
{	
	string Search;
	
	ref ObservableCollection<ref ScriptView> MenuItems = new ObservableCollection<ref ScriptView>(this);
	
	ref ObservableCollection<TreeView> LeftListItems = new ObservableCollection<TreeView>(this);
	ref ObservableCollection<TreeView> RightListItems = new ObservableCollection<TreeView>(this);
		
	ref ObservableCollection<ref EditorFileHeadView> OpenFiles = new ObservableCollection<ref EditorFileHeadView>(this);
		
	ScrollWidget LeftScroll;
	ScrollWidget RightScroll;	
	
	// on this day, 1/23, DayZ Editor achieved beauty
}