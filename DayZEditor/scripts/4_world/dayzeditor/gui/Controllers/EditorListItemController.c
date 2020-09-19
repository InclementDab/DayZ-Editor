


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
	protected static int COLOR_ON_SELECTED = COLOR_BLUE;
	protected static int COLOR_ON_DESELECTED = ARGB(140,35,35,35);
	protected static int COLOR_ON_HOVER = COLOR_SALMON;
	
	
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
		//EditorLog.Trace("EditorListItemController::OnMouseEnter");
		EditorListItemContent.SetColor(COLOR_ON_HOVER);
		
		switch (m_ListItem.Type()) {
			
			case EditorPlaceableListItem: {
				string item_type = EditorPlaceableListItem.Cast(m_ListItem).GetData().Type;
				EditorTooltip tooltip = new EditorTooltip();			
				tooltip.SetTitle(item_type);
				float pos_x, pos_y, size_x, size_y;
				m_LayoutRoot.GetScreenPos(pos_x, pos_y);
				m_LayoutRoot.GetScreenSize(size_x, size_y);
				tooltip.GetLayoutRoot().SetPos(pos_x + size_x, pos_y);
				tooltip.SetContent(GetGame().CreateObjectEx(item_type, vector.Zero, ECE_NONE));
				tooltip.Show();
				break;
			}		
		}
				
		return false;
	}
	
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		//EditorLog.Trace("EditorListItemController::OnMouseLeave");
		
		
		if (!m_Selected)
			EditorListItemContent.SetColor(COLOR_ON_DESELECTED);
		
		switch (m_ListItem.Type()) {
			case EditorPlaceableListItem: {
				if (GetEditorHudController().CurrentTooltip) {
					GetEditorHudController().CurrentTooltip.Close();
				}
				break;
			}
		}
		
		return false;
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
	


	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorListItemController::OnClick");
		

			
		switch (m_ListItem.Type()) {
			
			case EditorPlaceableListItem: {
				Select();
				if (button == 0) {
					GetEditor().CreateInHand(EditorPlaceableListItem.Cast(m_ListItem).GetData());	
				} else if (button == 1) {
					EditorPlaceableContextMenu placeable_context = new EditorPlaceableContextMenu();
					placeable_context.SetPosition(x, y);
					placeable_context.Show();
				}
				
				break;
			}
			
			case EditorPlacedListItem: {
				
				Select();
				if (button == 0) {
					EditorPlacedListItem placed_item = EditorPlacedListItem.Cast(m_ListItem);
					if (KeyState(KeyCode.KC_LCONTROL)) {
						GetEditor().ToggleSelection(placed_item.GetData());
					} else {
						GetEditor().ClearSelection();
						GetEditor().SelectObject(placed_item.GetData());
					}
				} else if (button == 1) {
					EditorContextMenu context_menu = new EditorContextMenu();
					context_menu.SetPosition(x, y);
					context_menu.Show();
				}
				break;
			}
		}
			
		
		
		return false;
	}
		
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorListItemController::OnDoubleClick");
		
		switch (m_ListItem.Type()) {
			
			case EditorPlacedListItem: {
				EditorObjectPropertiesDialog dialog();
				dialog.SetEditorObject(EditorPlacedListItem.Cast(m_ListItem).GetData());
				dialog.Show();
				break;
			}
		}
		
		return false;
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

	private bool m_Selected;
	void Select() {
		//EditorLog.Trace("EditorListItemController::Select");
		m_Selected = true;
		EditorListItemContent.SetColor(COLOR_ON_SELECTED);
		EditorListItemContent.Update();	
	}
	
	void Deselect() {	
		//EditorLog.Trace("EditorListItemController::Deselect");
		m_Selected = false;
		EditorListItemContent.SetColor(COLOR_ON_DESELECTED);
		EditorListItemContent.Update();	
	}
	
	bool IsSelected() {
		return m_Selected;
	}
	
	void SetLabel(string label) {
		EditorListItemLabel.SetText(label);
	}
	
	void SetIcon(string icon) {
		EditorListItemIcon.LoadImageFile(0, icon);
		EditorListItemIcon.SetImage(0);
	}
}

