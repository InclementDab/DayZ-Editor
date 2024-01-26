class EditorNodeViewController: ViewController
{
	ref ObservableCollection<EditorNodeView> ChildrenItems = new ObservableCollection<EditorNodeView>(this);
	
	Widget Collapse;
					
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{				
		Collapse.Show(ChildrenItems.Count() > 0);
	}
}