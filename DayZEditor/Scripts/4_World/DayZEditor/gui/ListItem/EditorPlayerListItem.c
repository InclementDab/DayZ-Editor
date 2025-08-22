class EditorPlayerListItem: EditorListItem
{
	protected string m_Name, m_Id;
	
	void EditorPlayerListItem(string id, string name)
	{
		m_Id = id;
		m_Name = name;
		
		ListItemLabel.SetText(string.Format("%1", m_Name));
	}
}