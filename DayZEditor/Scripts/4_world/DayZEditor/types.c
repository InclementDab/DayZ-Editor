
class EditorObjectMap: map<int, ref EditorObject>
{
	int InsertEditorObject(EditorObject editor_object)
	{
		return Insert(editor_object.GetID(), editor_object);
	}
	
	void RemoveEditorObject(EditorObject editor_object)
	{
		Remove(editor_object.GetID());
	}
}

class EditorObjectDataMap: map<int, ref EditorObjectData>
{
	int InsertData(EditorObjectData data)
	{
		return Insert(data.GetID(), data);
	}
	
	void RemoveData(EditorObjectData data)
	{
		Remove(data.GetID());
	}
}
