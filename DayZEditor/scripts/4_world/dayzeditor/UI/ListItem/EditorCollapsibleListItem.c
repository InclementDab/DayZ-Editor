// temp
static int groupcount = 0;
class EditorCollapsibleListItem: EditorListItem
{
	private bool m_Collapsed = false;

	private ref array<ref EditorListItem> m_CategoryChildren;
	
	void EditorCollapsibleListItem()
	{
		Print("EditorCollapsibleListItem");
		m_CategoryChildren = new array<ref EditorListItem>();
		
		SetText(string.Format("group%1", groupcount));
		groupcount++;
		
		//EditorEvents.OnObjectDeselected.Insert();
	}
	
	
	
	void ~EditorCollapsibleListItem()
	{
		Print("~EditorCollapsibleListItem");
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
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorCollapsibleListItem::OnClick");
	
		if (button == 0) {
			
			switch (w) {
				case m_ListItemCollapse: {
					m_Collapsed = !m_Collapsed;
					m_ListItemChildren.Show(!m_Collapsed);
					
					// temp
					if (m_Collapsed)
						m_ListItemCollapse.SetText(">");
					else
						m_ListItemCollapse.SetText("V");
					
					break;
				}
				case m_ListItemButton: {
					
					foreach (EditorListItem list_item: m_CategoryChildren) {
						list_item.Select();
					}
					
					break;
				}
			}			
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	
	override bool OnListItemDropReceived(EditorListItem target)
	{
		Print("EditorCollapsibleListItem::OnListItemDropReceived");
				
		target.SetNestIndex(GetNestIndex() + 1);
		m_CategoryChildren.Insert(target);
		m_ListItemChildren.AddChild(target.GetRoot());	
		
		return true;
	}
	
	override bool OnListItemDrop(EditorListItem target)
	{
		Print("EditorCollapsibleListItem::OnListItemDrop");
		
		if (ContainsChildListItem(target)) {
			return false;
		}
		return true;
	}
	
}