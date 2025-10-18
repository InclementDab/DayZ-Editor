class EditorPlayerListItem: EditorListItem
{
	int Id;
	protected string m_Name;
	
	void EditorPlayerListItem(int id, string name)
	{
		Id = id;
		m_Name = name;
		
		ListItemLabel.SetText(string.Format("%1", m_Name));
	}
}