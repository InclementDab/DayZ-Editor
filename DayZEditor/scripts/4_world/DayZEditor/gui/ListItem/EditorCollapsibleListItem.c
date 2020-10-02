

// todo: create EditorObjectGroup and handle this shit in there
// cause this isnt 100% working

// temp
static int groupcount = 0;
class EditorCollapsibleListItem: EditorListItem
{

	void EditorCollapsibleListItem(Widget parent = null) 
	{ 
		EditorLog.Trace("EditorCollapsibleListItem");
		
		EditorListItemCollapse.Show(true);
		m_Controller.ListItemLabel = string.Format("group%1", groupcount);
		m_Controller.NotifyPropertyChanged("ListItemLabel");
		groupcount++;
	}
	
	override void ListItemCollapseExecute(ButtonCommandArgs args)
	{
		EditorListItemChildren.Show(!args.GetButtonState());
	}
			
	
	override void ListItemExecute(ButtonCommandArgs args)
	{
		
		switch (args.GetMouseButton()) {
			
			case 0: {
				
				for (int i = 0; i < m_Controller.ChildListItems.Count(); i++) {
					
					EditorListItem list_item = m_Controller.ChildListItems[i];
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