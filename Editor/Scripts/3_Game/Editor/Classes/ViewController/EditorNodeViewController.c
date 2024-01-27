class TreeViewController: ViewController
{
	ref ObservableCollection<TreeView> ChildrenItems = new ObservableCollection<TreeView>(this);
	
	Widget Collapse;
					
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		Collapse.Show(ChildrenItems.Count() > 0);
	}
}