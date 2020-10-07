
class EditorObjectMap: map<int, ref EditorObject>
{
	int InsertEditorObject(EditorObject editor_object)
	{
		if (!editor_object) return -1;
		return Insert(editor_object.GetID(), editor_object);
	}
	
	void RemoveEditorObject(EditorObject editor_object)
	{
		if (!editor_object) return;
		Remove(editor_object.GetID());
	}
}

class EditorObjectDataMap: map<int, ref EditorObjectData>
{
	int InsertData(EditorObjectData data)
	{
		if (!data) return -1;
		return Insert(data.GetID(), data);
	}
	
	void RemoveData(EditorObjectData data)
	{
		if (!data) return;
		Remove(data.GetID());
	}
}
