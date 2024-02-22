class TreeViewController: ViewController
{
	ref ObservableCollection<ref NodeTreeView> ChildrenItems = new ObservableCollection<ref NodeTreeView>(this);
				
	Widget Collapse;
	
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		Collapse.Show(ChildrenItems.Count() > 0);
	}
}