class TreeViewController: ViewController
{
	ref ObservableCollectionNonRef<TreeView> ChildrenItems = new ObservableCollectionNonRef<TreeView>(this);
				
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		TreeView tree_view = TreeView.Cast(GetParent());
		if (tree_view) {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(tree_view.RecalculateSize);
		}
	}
}