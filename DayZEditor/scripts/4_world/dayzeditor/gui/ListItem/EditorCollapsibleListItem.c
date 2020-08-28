// temp
static int groupcount = 0;
class EditorCollapsibleListItem: EditorListItem
{
	
	private ref array<ref EditorListItem> m_CategoryChildren;
	ref array<ref EditorListItem> GetChildren() { return m_CategoryChildren; }
	
	void EditorCollapsibleListItem() 
	{ 
		EditorLog.Trace("EditorCollapsibleListItem");
		m_CategoryChildren = new array<ref EditorListItem>();
		
		m_LayoutRoot.FindAnyWidget("EditorListItemCollapse").Show(true);
		m_EditorListItemController.SetLabel(string.Format("group%1", groupcount));
		
		
		groupcount++;
		
	}
	
	

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
	}

	
}