class EditorFolderTreeItem: EditorTreeItem
{
	protected string m_Name;
	
	void EditorFolderTreeItem(string name)
	{
		m_Name = name;
		
		m_TemplateController.Text = name;
		m_TemplateController.NotifyPropertyChanged("Text");
		CollapseWrapper.Show(true);
	}
}