class EditorDeletedListItem: EditorListItem
{
	protected EditorDeletedObject m_EditorDeletedObject;
	
	void EditorDeletedListItem(EditorDeletedObject deleted_object)
	{
		m_EditorDeletedObject = deleted_object;
		
		m_TemplateController.Label = m_EditorDeletedObject.GetWorldObject().GetDisplayName();
		m_TemplateController.NotifyPropertyChanged("Label");
		
		//m_TemplateController.Icon = m_EditorDeletedObject.GetData().Icon;
		//m_TemplateController.NotifyPropertyChanged("Icon");
	}
}