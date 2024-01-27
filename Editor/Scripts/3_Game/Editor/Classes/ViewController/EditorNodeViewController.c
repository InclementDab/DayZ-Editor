class TreeViewController: ViewController
{
	ref ObservableCollection<ref TreeView> ChildrenItems = new ObservableCollection<ref TreeView>(this);
	
	Widget Collapse;
					
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		Collapse.Show(ChildrenItems.Count() > 0);
	}
}