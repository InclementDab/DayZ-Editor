class EditorTreeItemController: ViewController
{
	ref ObservableCollection<EditorTreeItem> Children = new ObservableCollection<EditorTreeItem>(this);
	
	Widget Icon, CollapseWrapper, ChildrenWrapper, WrapPadding;
		
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		Icon.Show(Children.Count() > 0);		
		CollapseWrapper.Show(Children.Count() > 0);
				
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
	}
}