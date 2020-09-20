// temp
static int groupcount = 0;
class EditorCollapsibleListItem: EditorListItem
{

	void EditorCollapsibleListItem(Widget parent = null) 
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