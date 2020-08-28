

class EditorListItemController: Controller
{
	
	Widget EditorListItemFrame;
	Widget EditorListItemLabelFrame;
	WrapSpacerWidget EditorListItemContent;
	WrapSpacerWidget EditorListItemChildren;
	
	ButtonWidget EditorListItemButton;
	ButtonWidget EditorListItemVisible;
	ButtonWidget EditorListItemCollapse;
	
	bool EditorListItemButtonData;
	bool EditorListItemCollapseData;
	bool EditorListItemVisibleData;
	string EditorListItemLabelData;
	string EditorListItemIconData;
	int EditorListItemContentColor;
	
	protected static int COLOR_ON_SELECTED = ARGB(140,41,128,185);
	protected static int COLOR_ON_DESELECTED = ARGB(140,35,35,35);
	
	ref ObservableCollection<ref EditorListItem> ChildListItems;
	
	void EditorListItemController()	{
		EditorLog.Trace("EditorListItemController");
	}
	
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		ChildListItems = new ObservableCollection<ref EditorListItem>("ChildListItems", this);
	}
	
	override void PropertyChanged(string property_name) 
	{
		
		switch (property_name) {
			
			case "EditorListItemContentColor": {
				EditorListItemContent.SetColor(EditorListItemContentColor);
				EditorListItemContent.Update();	
				break;
			}
			
			case "EditorListItemCollapseData": {
				EditorListItemChildren.Show(!EditorListItemCollapseData);
				
				// temp
				if (EditorListItemCollapseData)
					EditorListItemCollapse.SetText(">");
				else
					EditorListItemCollapse.SetText("V");
				
				break;
			}
		}
	}
	
		
	
	override void OnDrop(Widget target, Widget drop_target, int x, int y)
	{	
		EditorLog.Trace("EditorListItemController::OnDrop");
		
		EditorListItem drop_target_item;
		drop_target.GetUserData(drop_target_item);
		
		EditorListItem target_item;
		target.GetUserData(target_item);
		target_item.SetNestIndex(drop_target_item.GetNestIndex() + 1);
		drop_target_item.GetController().ChildListItems.Insert(target_item);
	}
	
	
	
	void EditorListItemButtonExecute(ButtonCommandArgs args)
	{
		
	}
	
	void EditorListItemCollapseExecute(ButtonCommandArgs args)
	{
		
	}
	
	void EditorListItemVisibleExecute(ButtonCommandArgs args)
	{
		
	}
	
		
	void Select() {
		EditorListItemContentColor = COLOR_ON_SELECTED;
		NotifyPropertyChanged("EditorListItemContentColor");
	}
	
	void Deselect() {	
		EditorListItemContentColor = COLOR_ON_DESELECTED;
		NotifyPropertyChanged("EditorListItemContentColor");
	}
	
	
	void SelectAllChildren()
	{
		for (int i = 0; i < ChildListItems.Count(); i++) {
			ChildListItems[i].GetController().Select();
			ChildListItems[i].GetController().SelectAllChildren();
		}
	}

	
}

