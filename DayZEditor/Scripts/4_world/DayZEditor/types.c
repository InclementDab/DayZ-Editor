class EditorObjectSet: ref map<int, ref EditorObject>
{
	
	void ~EditorObjectSet() {
		EditorLog.Trace("~EditorObjectSet");
	}
	
	bool InsertEditorObject(EditorObject target) { 
		return Insert(target.GetID(), target); 
	}
	
	void RemoveEditorObject(EditorObject target) { 
		Remove(target.GetID()); 
	}
}