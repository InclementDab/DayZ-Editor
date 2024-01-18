class EditorTreeItemController: ViewController
{
	ref ObservableCollection<EditorTreeItem> Children = new ObservableCollection<EditorTreeItem>(this);
	
	Widget Icon, Spacer1, CollapseWrapper, Spacer0, ChildrenWrapper;
		
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		Icon.Show(Children.Count() > 0);
		Spacer0.Show(Children.Count() > 0);
		
		CollapseWrapper.Show(Children.Count() > 0);
		Spacer1.Show(Children.Count() > 0);
		
		float w, h;
		m_LayoutRoot.GetScreenSize(w, h);
		m_LayoutRoot.SetScreenSize(w, Ternary<float>.If(ChildrenWrapper.IsVisible(), 18 * (Children.Count()) + 18, 18));
		
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