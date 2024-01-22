class EditorCategory: EditorNode
{	
	static const string DEFAULT_ICON = "set:icons image:folder";
	
	protected string m_Name;
	protected string m_Icon;
	
	void EditorCategory(string uuid, string display_name, string icon = DEFAULT_ICON)
	{
		m_Icon = icon;
	}
		
	string GetIcon()
	{
		return m_Icon;
	}
}