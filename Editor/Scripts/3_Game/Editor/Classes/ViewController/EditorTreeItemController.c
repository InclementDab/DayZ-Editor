class EditorNodeViewController: ViewController
{
	ref ObservableCollection<EditorNodeView> ChildrenItems = new ObservableCollection<EditorNodeView>(this);
		
	Widget Collapse;
		
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{			
		Param1<EditorNodeView> item = Param1<EditorNodeView>.Cast(args.ChangedValue);
		EditorNodeView script_view = EditorNodeView.Cast(GetParent());
		switch (args.ChangedAction) {
			case NotifyCollectionChangedAction.Insert:
			case NotifyCollectionChangedAction.InsertAt: {
				item.param1.SetParentTree(script_view);
				break;
			}
			
			case NotifyCollectionChangedAction.Remove: {
				item.param1.SetParentTree(null);
				break;
			}
		}
				
		Collapse.Show(ChildrenItems.Count() > 0);
	}
}