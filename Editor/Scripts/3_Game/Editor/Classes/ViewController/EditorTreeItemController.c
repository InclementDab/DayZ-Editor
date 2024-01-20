class EditorTreeItemController: ViewController
{
	ref ObservableCollection<EditorTreeItem> ChildrenItems = new ObservableCollection<EditorTreeItem>(this);
	
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
		Param1<EditorTreeItem> item = Param1<EditorTreeItem>.Cast(args.ChangedValue);
		EditorTreeItem script_view = EditorTreeItem.Cast(GetParent());
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