class EditorFolderTreeItem: EditorTreeItem
{
	protected string m_Name;
	
	void EditorFolderTreeItem(string name)
	{
		m_Name = name;
		
		m_TemplateController.Text = name;
		m_TemplateController.NotifyPropertyChanged("Text");
		CollapseWrapper.Show(false);
		Spacer0.Show(false);
	}
	
	void ApplyFilter(string filter)
	{
		Show(false);
		filter.ToLower();
		
		bool has_visible_children;
		for (int i = 0; i < m_TemplateController.Children.Count(); i++) {
			
			string name = m_TemplateController.Children[i].GetTemplateController().Text;
			name.ToLower();
			
			m_TemplateController.Children[i].Show(name.Contains(filter));
			if (name.Contains(filter)) {
				Show(true);
			}
		}
	}
}