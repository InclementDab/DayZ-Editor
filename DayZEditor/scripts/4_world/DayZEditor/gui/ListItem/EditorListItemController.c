
class EditorListItemController: Controller
{
	private EditorListItem m_ListItem;
	void SetListItem(EditorListItem list_item) {
		m_ListItem = list_item;		
	}
	
	string ListItemLabel;
	string ListItemIcon;
	
	ref ObservableCollection<ref EditorListItem> ChildListItems;
	
	void EditorListItemController()	{
		EditorLog.Trace("EditorListItemController");
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("EditorListItemController::OnWidgetScriptInit");
		super.OnWidgetScriptInit(w);
		
		ChildListItems = new ObservableCollection<ref EditorListItem>("ChildListItems", this);
	}

	void ListItemExecute(ButtonCommandArgs args) {
		m_ListItem.ListItemExecute(args);
	}
	
	void ListItemVisibleExecute(ButtonCommandArgs args) {
		m_ListItem.ListItemVisibleExecute(args);
	}
	
	void ListItemCollapseExecute(ButtonCommandArgs args) {
		m_ListItem.ListItemCollapseExecute(args);
	}
	/*
	override void MVCOnDrag(Widget target, int x, int y)
	{
		//EditorLog.Trace("EditorListItemController::OnDrag");
	}*/
	/*
	override void OnDrop(Widget target, Widget drop_target, int x, int y)
	{	
		return;
		EditorLog.Trace("EditorListItemController::OnDrop");
		
		EditorListItem drop_target_item;
		drop_target.GetUserData(drop_target_item);
		
		EditorListItem target_item;
		target.GetUserData(target_item);
		target_item.SetNestIndex(drop_target_item.GetNestIndex() + 1);
		drop_target_item.GetController().ChildListItems.Insert(target_item);
	}*/
	/*
	override bool OnDrag(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorListItemController::OnDrag");
		

		switch (m_ListItem.Type()) {
			
			case EditorPlaceableListItem: {
				GetEditor().CreateInHand(EditorPlaceableListItem.Cast(m_ListItem).GetData());
				break;
			}
			
			case EditorPlacedListItem: {
				break;
			}
			
			case EditorCollapsibleListItem: {				
				return false;
			}
		}
		
		
		return super.OnDrag(w, x, y);
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		EditorLog.Trace("EditorListItemController::OnDrop");
		
		switch (m_ListItem.Type()) {
			
			case EditorPlaceableListItem: {
				GetEditor().PlaceObject();
				break;
			}
			
			case EditorPlacedListItem: {
				return false;
			}
			
			case EditorCollapsibleListItem: {				
				return false;
			}
			
			
		}
		
		return false;
	}
	
	*/
	
	/*
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorListItemController::OnDoubleClick");
		
		switch (m_ListItem.Type()) {
			
			case EditorPlacedListItem: {
				EditorObjectPropertiesDialog dialog();
				dialog.SetEditorObject(EditorPlacedListItem.Cast(m_ListItem).GetData());
				dialog.ShowDialog();
				break;
			}
		}
		
		return false;
	}
	
	
	*/
	void SelectAllChildren()
	{
		for (int i = 0; i < ChildListItems.Count(); i++) {
			//ChildListItems[i].GetController().Select();
			//ChildListItems[i].GetController().SelectAllChildren();
		}
	}
}

