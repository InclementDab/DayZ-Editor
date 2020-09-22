

// todo: create EditorObjectGroup and handle this shit in there
// cause this isnt 100% working

// temp
static int groupcount = 0;
class EditorCollapsibleListItem: EditorListItem
{

	void EditorCollapsibleListItem(ScriptView parent = null) 
	{ 
		EditorLog.Trace("EditorCollapsibleListItem");
		
		EditorListItemCollapse.Show(true);
		GetListItemController().ListItemLabel = string.Format("group%1", groupcount);
		GetListItemController().NotifyPropertyChanged("ListItemLabel");
		groupcount++;
	}
	
	override void ListItemCollapseExecute(ButtonCommandArgs args)
	{
		EditorListItemChildren.Show(!args.GetButtonState());
	}
	
	void AddListItem(EditorListItem item)
	{
		EditorLog.Trace("EditorCollapsibleListItem::AddListItem");
		
		GetListItemController().ChildListItems.Insert(item);
		item.SetNestIndex(m_NestIndex + 1);
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		EditorLog.Trace("EditorCollapsibleListItem::OnDrop");
		RecursiveGetParent(reciever, "EditorListItem");
		EditorListItem target_item = GetListItemFromWidget(reciever);
		if (!target_item) {
			return false;
		}
		
		switch (target_item.Type()) {
			
			case EditorPlacedListItem: {
				
				break;
			}
			
			case EditorCollapsibleListItem: {
				EditorCollapsibleListItem.Cast(target_item).AddListItem(this);
				break;
			}			
		}
		
		return true;
	}
	
	
	override void ListItemExecute(ButtonCommandArgs args)
	{
		
		switch (args.GetMouseButton()) {
			
			case 0: {
				
				for (int i = 0; i < GetListItemController().ChildListItems.Count(); i++) {
					
					ScriptView list_item = GetListItemController().ChildListItems[i];
					switch (list_item.Type()) {
						
						case EditorPlacedListItem: {
							GetEditor().SelectObject(EditorPlacedListItem.Cast(list_item).GetData());
							break;
						}
						
						case EditorCollapsibleListItem: {
							EditorCollapsibleListItem.Cast(list_item).ListItemExecute(args);
							break;
						}
						
					}
					
				}
				
				break;
			}
			
		}
	}
	
/*
	void RemoveListItem(EditorListItem item)
	{
		int index = m_CategoryChildren.Find(item);
		if (index == -1) return;
		m_CategoryChildren.Remove(index);
	}
	
	bool ContainsChildListItem(EditorListItem item)
	{
		foreach (EditorListItem child: m_CategoryChildren) {
			
			// Check if child is collapsible
			EditorCollapsibleListItem collapsible;
			if (CastTo(collapsible, child)) {
				return collapsible.ContainsChildListItem(item);
			} 
			else if (child == item) {
				return true;
			}
		}
		
		return false;
	}
	
	override void SetNestIndex(int index)
	{
		super.SetNestIndex(index);
		foreach (EditorListItem item: m_CategoryChildren) {
			item.SetNestIndex(index + 1);
		}
	}*/

	
}