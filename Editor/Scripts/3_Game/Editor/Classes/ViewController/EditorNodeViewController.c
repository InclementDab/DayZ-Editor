class EditorNodeViewController: ViewController
{
	ref ObservableCollection<EditorNodeView> ChildrenItems = new ObservableCollection<EditorNodeView>(this);
	
	bool CollapseState;
	Widget Collapse;
	
	ImageWidget CollapseIcon;
	WrapSpacerWidget Children;
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "CollapseState": {
				if (!CollapseIcon) {
					break;
				}
				
				CollapseIcon.LoadImageFile(0, Ternary<string>.If(!CollapseState, "set:dayz_gui image:Expand", "set:dayz_gui image:Collapse"));
				CollapseIcon.SetImage(0);
				
				float w, h;
				Children.Show(CollapseState);
				Children.GetScreenSize(w, h);
				//Print(h);
				float x, y;
				m_LayoutRoot.GetSize(x, y);
				m_LayoutRoot.SetScreenSize(x, 18 + (h * CollapseState));				
				break;
			}
		}
	}
			
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{				
		Collapse.Show(ChildrenItems.Count() > 0);
	}
}