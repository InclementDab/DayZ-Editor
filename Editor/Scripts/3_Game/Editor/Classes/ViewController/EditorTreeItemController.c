class EditorNodeViewController: ViewController
{
	ref ObservableCollection<EditorNodeView> ChildrenItems = new ObservableCollection<EditorNodeView>(this);
	
	string IconImage, CollapseIcon;
	
	Widget Icon, Collapse;
		
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "IconImage": {
				Icon.Show(IconImage != string.Empty);
				break;
			}
		}
	}
	
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