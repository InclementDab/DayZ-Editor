class EditorFindAndReplaceCommand: EditorAsyncCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		EditorFindAndReplaceDialog dialog(GetName());
		dialog.ShowDialog();
	}
	
	array<EditorObject> FindObjects(string query)
	{
		query.ToLower();
		array<EditorObject> found_objects = {};
		array<EditorObject> editor_objects = GetEditor().GetPlacedObjects();
		foreach (EditorObject editor_object: editor_objects) {
			string t1 = editor_object.GetWorldObject().GetType();
			t1.ToLower();
			if (t1 != query) {
				continue;
			}
			
			found_objects.Insert(editor_object);
		}
		
		return found_objects;
	}
	
	array<EditorObject> FindAndReplaceObjects(string query, string replace)
	{
		/*array<ref EditorObjectData> created_data = {};
		array<EditorObject> found_objects = FindObjects(query);
		foreach (EditorObject editor_object: found_objects) {
			EditorObjectData data = editor_object.CreateSerializedData();
			data.Type = replace;
			created_data.Insert(data);
		}
		
		GetEditor().DeleteObjects(found_objects);
		return GetEditor().CreateObjects(created_data);*/
		return {};
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_CMD_FIND_AND_REPLACE";
	}
}