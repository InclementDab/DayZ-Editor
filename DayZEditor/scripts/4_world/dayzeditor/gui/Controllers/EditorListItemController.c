


class EditorListItemController: Controller
{
	private typename m_ListItemType;	
	private EditorListItem m_ListItem;
	void SetListItem(EditorListItem list_item) {
		m_ListItem = list_item;
		m_ListItemType = m_ListItem.Type();
	}
	
	

	WrapSpacerWidget EditorListItemContent;
	WrapSpacerWidget EditorListItemChildren;
	
	bool EditorListItemButton;
	bool EditorListItemCollapse;
	bool EditorListItemVisible;
	string EditorListItemLabel;
	string EditorListItemIcon;
	string EditorListItemCollapseText;
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
		EditorLog.Trace("EditorListItemController::EditorListItemButtonExecute");
		
		switch (m_ListItemType) {
			
			case EditorPlaceableListItem: {
				GetEditor().CreateInHand(EditorPlaceableListItem.Cast(m_ListItem).GetData());
				break;
			}
			
		}
		
	}
	
	void EditorListItemCollapseExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorListItemController::EditorListItemCollapseExecute");
		
		switch (m_ListItemType) {
			
			case EditorCollapsibleListItem: {
				EditorListItemChildren.Show(!args.param3);
				
				// temp
				if (args.param3)
					EditorListItemCollapseText = ">";
				else
					EditorListItemCollapseText = "V";
				
				NotifyPropertyChanged("EditorListItemCollapseText");
				break;
			}
			
		}
		
		
				

	}
	
	void EditorListItemVisibleExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorListItemController::EditorListItemVisibleExecute");
	}
	
		
	void Select() {
		EditorLog.Trace("EditorListItemController::Select");
		EditorListItemContentColor = COLOR_ON_SELECTED;
		NotifyPropertyChanged("EditorListItemContentColor");
	}
	
	void Deselect() {	
		EditorLog.Trace("EditorListItemController::Deselect");
		EditorListItemContentColor = COLOR_ON_DESELECTED;
		NotifyPropertyChanged("EditorListItemContentColor");
	}
	
	void SetLabel(string label) {
		EditorListItemLabel = label;
		NotifyPropertyChanged("EditorListItemLabelData");
	}
	
	void SetIcon(string icon) {
		EditorListItemIcon = icon;
		NotifyPropertyChanged("EditorListItemIconData");
	}
	
	void SelectAllChildren()
	{
		for (int i = 0; i < ChildListItems.Count(); i++) {
			ChildListItems[i].GetController().Select();
			ChildListItems[i].GetController().SelectAllChildren();
		}
	}

	
}

