class EditorNodeViewController: ViewController
{
	ref ObservableCollection<ref EditorNodeView> ChildrenItems = new ObservableCollection<ref EditorNodeView>(this);
	
	Widget Collapse;
					
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{				
		Collapse.Show(ChildrenItems.Count() > 0);
	}
}