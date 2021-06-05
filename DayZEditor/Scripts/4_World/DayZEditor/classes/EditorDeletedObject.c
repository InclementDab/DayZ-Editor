class EditorDeletedObject: EditorWorldObject
{
	void EditorDeletedObject(Object object)
	{
		m_WorldObject = object;
	}
	
	int GetID()
	{
		return m_WorldObject.GetID();
	}
	
	void Hide(bool update_pathgraph = false)
	{
		CF.ObjectManager.HideMapObject(m_WorldObject, update_pathgraph);
	}
	
	void Show(bool update_pathgraph = false)
	{
		CF.ObjectManager.UnhideMapObject(m_WorldObject, update_pathgraph);
	}
}