


class EditorListItemController: Controller
{

	private EditorListItem m_ListItem;
	void SetListItem(EditorListItem list_item) {
		m_ListItem = list_item;
	}
	
	protected Widget EditorListItemHighlight;
	protected Widget EditorListItemContent;
	protected TextWidget EditorListItemLabel;
	protected ImageWidget EditorListItemIcon;
	protected WrapSpacerWidget EditorListItemChildren;
	
	//protected static int COLOR_ON_SELECTED = ARGB(140,41,128,185);
	protected static int COLOR_ON_SELECTED = COLOR_SALMON;
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
	

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorListItemController::OnMouseEnter");
		
		switch (m_ListItem.Type()) {
			default: {
				//w.SetColor(COLOR_SALMON);
				EditorListItemHighlight.Show(true);
				break;
			}			
		}
				
		return false;
	}
	
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		EditorLog.Trace("EditorListItemController::OnMouseLeave");
		
		
		switch (m_ListItem.Type()) {
			
			default: {
				//w.SetColor(COLOR_EMPTY);
				EditorListItemHighlight.Show(false);
				break;
			}
		}
		
		return false;
	}
	
	override void MVCOnDrag(Widget target, int x, int y)
	{
		//EditorLog.Trace("EditorListItemController::OnDrag");
	}
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
	
	
	override void MVCOnClick(Widget target, int button, int x, int y)
	{
		EditorLog.Trace("EditorListItemController::MVCOnClick");
		
	
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
	
	override void MVCOnDoubleClick(Widget target, int button, int x, int y)
	{
		EditorLog.Trace("EditorListItemController::MVCOnDoubleClick");
		
		switch (m_ListItem.Type()) {
			
			
			case EditorPlacedListItem: {
				EditorObjectPropertiesDialog dialog(EditorPlacedListItem.Cast(m_ListItem).GetData());
				dialog.ShowDialog();
				break;
			}
			
			
		}
	}
	
	
	void EditorListItemCollapseExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorListItemController::EditorListItemCollapseExecute");
		
		switch (m_ListItem.Type()) {
			
			case EditorCollapsibleListItem: {
				EditorListItemChildren.Show(!args.param3);
				
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

