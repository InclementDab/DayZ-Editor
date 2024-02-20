static int EditorObjectDataId;
modded class EditorObjectData
{
	protected int m_Id;
	
	override int GetID()
	{
		return m_Id;
	}
	
	void EditorObjectData()
	{
		EditorObjectDataId++;
		m_Id = EditorObjectDataId;
	}
}