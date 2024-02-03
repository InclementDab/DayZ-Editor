class TreeViewController: ViewController
{
	ref ObservableCollectionNonRef<TreeView> ChildrenItems = new ObservableCollectionNonRef<TreeView>(this);
				
	Widget Collapse;
	
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		Collapse.Show(ChildrenItems.Count() > 0);
	}
}