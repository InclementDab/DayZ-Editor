class EditorTreeItemController: ViewController
{
	ref ObservableCollection<EditorTreeItem> Children = new ObservableCollection<EditorTreeItem>(this);
	
	Widget Icon, CollapseWrapper, ChildrenWrapper, WrapPadding;
			
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		Icon.Show(Children.Count() > 0);		
		CollapseWrapper.Show(Children.Count() > 0);
				
		float w, h, c_w, c_h;
		Param1<EditorTreeItem> item = Param1<EditorTreeItem>.Cast(args.ChangedValue);
		EditorTreeItem script_view = EditorTreeItem.Cast(GetParent());
		switch (args.ChangedAction) {
			case NotifyCollectionChangedAction.Insert:
			case NotifyCollectionChangedAction.InsertAt: {				
				ChildrenWrapper.GetScreenSize(w, h);
				item.param1.GetLayoutRoot().GetScreenSize(c_w, c_h);
				ChildrenWrapper.SetScreenSize(w, h + c_h);
				
				item.param1.SetParentTree(script_view);
				break;
			}
			
			case NotifyCollectionChangedAction.Remove: {
				item.param1.SetParentTree(null);
				ChildrenWrapper.GetScreenSize(w, h);
				item.param1.GetLayoutRoot().GetScreenSize(c_w, c_h);
				ChildrenWrapper.SetScreenSize(w, h + c_h);
				break;
			}
		}
	}
}