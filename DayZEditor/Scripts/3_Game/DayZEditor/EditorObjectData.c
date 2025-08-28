static int EditorObjectDataId;
modded class EditorObjectData
{
	[NonSerialized()]
	protected int m_Id = EditorObjectDataId++;
	
	override int GetID()
	{
		return m_Id;
	}
	
	// Safely create a copy to clear reference counts
	/* todo "safely" isnt applied here yet...
	EditorObjectData CreateCopy()
	{
		EditorObjectData copied_data = SerializeCopy<EditorObjectData>.CreateCopy(this, int.MAX);
		copied_data.m_Id = m_Id;
		return copied_data;
	}*/
}