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
	}
	
	void ~EditorCollapsibleListItem()
	{
		Print("~EditorCollapsibleListItem");
	}
	
	void InsertListItem(EditorListItem item)
	{
		Print("InsertListItem");
		item.SetNestIndex(GetNestIndex() + 1);
		m_CategoryChildren.Insert(item);
		m_ListItemChildren.AddChild(item.GetRoot());
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
			}			
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	
	override bool OnListItemDropReceived(EditorListItem target)
	{
		
		if (ContainsChildListItem(target)) {
			return true;
		} else {
			InsertListItem(target);
		}
		
		return true;
	}
	
}