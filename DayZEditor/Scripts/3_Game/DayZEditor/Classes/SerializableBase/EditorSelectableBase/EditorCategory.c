class EditorCategory: EditorSelectableBase
{
	// todo maybe ref this here from now on
	protected ref array<EditorSelectableBase> m_Children = {};
	
	protected string m_Name;
	
	void EditorCategory(string name)
	{
		m_Name = name;
		m_TreeItem = new EditorTreeItem(m_Name, this);
	}
	
	override string GetName()
	{
		return m_Name;
	}
		
	void AddChild(notnull EditorSelectableBase child)
	{
		m_Children.Insert(child);
		
		m_TreeItem.GetTemplateController().Children.Insert(child.GetTreeItem());
	}
	
	void RemoveChild(notnull EditorSelectableBase child)
	{
		m_Children.RemoveItem(child);
	}
	
	override EditorTreeItem GetTreeItem()
	{
		return m_TreeItem;
	}
}