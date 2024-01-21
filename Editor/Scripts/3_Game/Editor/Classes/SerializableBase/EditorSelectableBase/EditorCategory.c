class EditorCategory: EditorNode
{
	// todo maybe ref this here from now on
	protected ref array<EditorNode> m_Children = {};
	
	protected EditorObjectDataCategory m_Category;
	
	void EditorCategory(EditorObjectDataCategory category)
	{
		m_Category = category;
		m_TreeItem = new EditorTreeItem(m_Category.GetName(), this);
		m_TreeItem.SetIcon(m_Category.GetIcon());
	}
	
	override void SetLocked(bool locked)
	{
		super.SetLocked(locked);
		
		//m_TreeItem.SetIcon();
	}
	
	override string GetName()
	{
		return m_Category.GetName();
	}
		
	void AddChild(notnull EditorNode child)
	{
		m_Children.Insert(child);
		
		m_TreeItem.GetTemplateController().ChildrenItems.Insert(child.GetTreeItem());
	}
	
	void RemoveChild(notnull EditorNode child)
	{
		m_Children.RemoveItem(child);
	}
	
	override EditorTreeItem GetTreeItem()
	{
		return m_TreeItem;
	}
}