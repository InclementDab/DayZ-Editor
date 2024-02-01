class TreeViewController: ViewController
{
	ref ObservableCollection<TreeView> ChildrenItems = new ObservableCollection<TreeView>(this);
	
	Widget Collapse, Texture;
					
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		Collapse.Show(ChildrenItems.Count() > 0);
		Texture.Update();
		
		TreeView tree_view = TreeView.Cast(GetParent());
		if (tree_view) {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(tree_view.RecalculateSize);
		}
	}
}