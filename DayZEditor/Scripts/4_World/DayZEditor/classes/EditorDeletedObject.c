class EditorDeletedObject: EditorWorldObject
{
	protected vector m_OriginalPosition;
	protected vector m_OriginalOrientation;
	protected ref EditorDeletedListItem m_EditorDeletedListItem;
	
	void EditorDeletedObject(notnull Object object)
	{
		m_WorldObject = object;
		m_OriginalPosition = m_WorldObject.GetPosition();
		m_OriginalOrientation = m_WorldObject.GetOrientation();
		
		// todo: probably use the events system to insert this stuff into the UI
		// use GetListItem()
		m_EditorDeletedListItem = new EditorDeletedListItem(this);
		GetEditor().GetEditorHud().GetTemplateController().RightbarDeletionData.Insert(m_EditorDeletedListItem);
		
		CF.ObjectManager.HideMapObject(m_WorldObject);
	}
	
	void ~EditorDeletedObject()
	{
		CF.ObjectManager.UnhideMapObject(m_WorldObject);
		delete m_EditorDeletedListItem;
	}
	
	string GetType()
	{
		return m_WorldObject.GetType();
	}
	
	int GetID()
	{
		return m_WorldObject.GetID();
	}
	
	EditorDeletedListItem GetListItem()
	{
		return m_EditorDeletedListItem;
	}
}