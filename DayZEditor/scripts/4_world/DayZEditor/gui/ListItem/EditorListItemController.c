
class EditorListItemController: Controller
{
	private EditorListItem m_ListItem;
	void SetListItem(EditorListItem list_item) {
		m_ListItem = list_item;		
	}
	
	string ListItemLabel;
	string ListItemIcon;
	
	ref ObservableCollection<ref EditorListItem> ChildListItems = new ObservableCollection<ref EditorListItem>("ChildListItems", this);
		
	void EditorListItemController()	{
		EditorLog.Trace("EditorListItemController");
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("EditorListItemController::OnWidgetScriptInit");
		super.OnWidgetScriptInit(w);
	}
	
	// janky way to try to get the group head to select
	/*
	override bool OnEvent(EventType eventType, Widget target, int parameter0, int parameter1)
	{
		if (m_ListItem.Type() == EditorCollapsibleListItem && ChildListItems.Count() > 0) {
			if (AllChildrenSelected()) {
				m_ListItem.Select();
			} else {
				m_ListItem.Deselect();
			}
		}
		return super.OnEvent(eventType, target, parameter0, parameter1);
	}
*/
	
	private bool AllChildrenSelected()
	{
		for (int i = 0; i < ChildListItems.Count(); i++) {
			EditorListItem list_item = EditorListItem.Cast(ChildListItems[i]);
			if (!list_item.IsSelected()) {
				return false;
			}
		}
		
		return true;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		return m_ListItem.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return m_ListItem.OnMouseLeave(w, enterW, x, y);
	}
	
	override bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		if (reciever == m_ListItem.EditorListItemButton && m_ListItem.Type() == EditorCollapsibleListItem) {
			ScriptedViewBase scripted_view;
			w.GetScript(scripted_view);
			ScriptedViewBase.FindScriptedRoot(scripted_view);
			
			reciever = m_ListItem.EditorListItemChildren;
			ViewBinding view_binding;
			reciever.GetScript(view_binding);
			
			if (view_binding) {
				//Print(ChildListItems.Count());
				//Print(m_ListItem.GetListItemController().ChildListItems.Count());
				//Print(EditorListItem.Cast(scripted_view).GetListItemController().ChildListItems.Count());
				//Print(EditorListItem.Cast().GetListItemController().ChildListItems.Count());
				
				// Checks for the child-in-parent deal
				if (!RecursiveCheckChildren(m_ListItem, ChildListItems)) {
					view_binding.OnDropReceived(w, x, y, reciever);
				}
				return true;
			}
		}
				
		return false;
	}
	
	private bool RecursiveCheckChildren(EditorListItem target, out ObservableCollection<ref EditorListItem> check_list)
	{	
		Print(check_list.Count());
		for (int i = 0; i < check_list.Count(); i++) {
			Print(check_list[i]);
			/*if (check_list[i] == target) {
				return true;
			}
			
			if (check_list[i].Type() == EditorCollapsibleListItem) {
				if (RecursiveCheckChildren(target, check_list[i].GetListItemController().ChildListItems)) {
					return true;
				}
			}*/
		}
		
		return false;
	}
}

