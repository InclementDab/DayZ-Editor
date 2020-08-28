


class EditorListItemController: Controller
{

	private EditorListItem m_ListItem;
	void SetListItem(EditorListItem list_item) {
		m_ListItem = list_item;
	}
	
	protected Widget EditorListItemContent;
	protected TextWidget EditorListItemLabel;
	protected ImageWidget EditorListItemIcon;
	protected WrapSpacerWidget EditorListItemChildren;
	
	protected static int COLOR_ON_SELECTED = ARGB(140,41,128,185);
	protected static int COLOR_ON_DESELECTED = ARGB(140,35,35,35);

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
	
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		return false;
	}
	
	
	override bool OnDrag(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorListItemController::OnDrag");
		
		return false;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{	
		EditorLog.Trace("EditorListItemController::OnDrop");
		
		EditorListItem target_item;
		w.GetUserData(target_item);
	
		if (reciever) {
			
			EditorListItem drop_target;
			reciever.GetUserData(drop_target);
			
			switch (drop_target.Type()) {
				
				case EditorCollapsibleListItem: {
					target_item.SetNestIndex(drop_target.GetNestIndex() + 1);
					drop_target.GetController().ChildListItems.Insert(target_item);
					break;
				}
				
				default: {
					// use the AddChildAfter stuff once you get the observable thing
					
				}
				
			}
		} else {
			// use the AddChildAfter stuff once you get the observable thing
			
		}
		
		

		return false;
	}
	
	
	override void OnClick(ViewBinding target, int button, int x, int y)
	{
		EditorLog.Trace("EditorListItemController::OnClick %1", target.GetBindingName());
		switch (m_ListItem.Type()) {
			
			case EditorPlaceableListItem: {
				GetEditor().CreateInHand(EditorPlaceableListItem.Cast(m_ListItem).GetData());
				Select();
				break;
			}
			
			case EditorPlacedListItem: {
				EditorPlacedListItem placed_item = EditorPlacedListItem.Cast(m_ListItem);
				if (KeyState(KeyCode.KC_LCONTROL)) {
					GetEditor().ToggleSelection(placed_item.GetData());
				} else {
					GetEditor().ClearSelection();
					GetEditor().SelectObject(placed_item.GetData());
				}
				break;
			}
			
			
		}
		
	}
	
	
	void EditorListItemCollapseExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorListItemController::EditorListItemCollapseExecute");
		
		EditorListItemChildren.Show(!args.param3);
		EditorListItemChildren.Update();
		
		switch (m_ListItem.Type()) {
			
			case EditorCollapsibleListItem: {
				
				
				// temp
				/*
				if (args.param3)
					EditorListItemCollapseText = ">";
				else
					EditorListItemCollapseText = "V";
				
				NotifyPropertyChanged("EditorListItemCollapseText");*/
				break;
			}
			
		}
	}
	
	void EditorListItemVisibleExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorListItemController::EditorListItemVisibleExecute");
	}
	
	
	void SelectAllChildren()
	{
		for (int i = 0; i < ChildListItems.Count(); i++) {
			ChildListItems[i].GetController().Select();
			ChildListItems[i].GetController().SelectAllChildren();
		}
	}

	void Select() {
		//EditorLog.Trace("EditorListItemController::Select");
		EditorListItemContent.SetColor(COLOR_ON_SELECTED);
		EditorListItemContent.Update();	
	}
	
	void Deselect() {	
		//EditorLog.Trace("EditorListItemController::Deselect");
		EditorListItemContent.SetColor(COLOR_ON_DESELECTED);
		EditorListItemContent.Update();	
	}
	
	void SetLabel(string label) {
		EditorListItemLabel.SetText(label);
	}
	
	void SetIcon(string icon) {
		EditorListItemIcon.LoadImageFile(0, icon);
		EditorListItemIcon.SetImage(0);
	}


	
}

