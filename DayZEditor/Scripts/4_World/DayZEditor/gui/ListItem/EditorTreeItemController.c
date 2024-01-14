class EditorTreeItemController: ViewController
{
	ref ObservableCollection<ref EditorTreeItem> Children = new ObservableCollection<ref EditorTreeItem>(this);
	
	string Text;
	
	Widget Panel;
	Widget Icon;
	Widget CollapseWrapper;
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "Text": {
				SizeToChild size_to_child;
				Panel.GetScript(size_to_child);
				size_to_child.ResizeParentToChild();
				break;
			}
		}
	}
	
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
				item.param1.ParentDisplay.Show(true);
				break;
			}
			
			case NotifyCollectionChangedAction.Remove: {
				item.param1.SetParentTree(null);
				break;
			}
		}
	}
}