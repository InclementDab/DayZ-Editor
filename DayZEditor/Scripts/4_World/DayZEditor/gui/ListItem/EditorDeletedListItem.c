class EditorDeletedListItem: EditorListItem
{
	protected EditorDeletedObject m_EditorDeletedObject;
	
	void EditorDeletedListItem(EditorDeletedObject deleted_object)
	{
		m_EditorDeletedObject = deleted_object;
		
		string item_name = m_EditorDeletedObject.GetType();
		if (item_name.Length() >= 18) {
			item_name = item_name.Substring(0, 15) + "...";
		}
		
		m_TemplateController.Label = string.Format("%1 (%2)", item_name, m_EditorDeletedObject.GetID());
		m_TemplateController.NotifyPropertyChanged("Label");
		
		//m_TemplateController.Icon = m_EditorDeletedObject.GetData().Icon;
		//m_TemplateController.NotifyPropertyChanged("Icon");
	}
}