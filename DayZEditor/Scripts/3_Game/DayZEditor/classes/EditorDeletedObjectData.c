class EditorDeletedObjectData
{
	string Name;
	vector Position;
	
	// Exists for the soul purpose of backwards compatibility
	static EditorDeletedObjectData CreateFromId(int id)
	{
		return new EditorDeletedObjectData();
	}
}