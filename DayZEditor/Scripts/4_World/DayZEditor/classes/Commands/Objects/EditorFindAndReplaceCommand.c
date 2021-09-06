class EditorFindAndReplaceCommand: EditorAsyncCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		EditorFindAndReplaceDialog dialog(GetName());
		dialog.ShowDialog();
	}
	
	EditorObjectMap FindObjects(string query)
	{
		query.ToLower();
		EditorObjectMap found_objects();
		EditorObjectMap editor_objects = GetEditor().GetPlacedObjects();
		foreach (int _, EditorObject editor_object: editor_objects) {
			string t1 = editor_object.GetType();
			t1.ToLower();
			if (t1 != query) {
				continue;
			}
			
			found_objects.InsertEditorObject(editor_object);
		}
		
		return found_objects;
	}
	
	//todo maybe have action for Replace, and create a replace lambda
	EditorObjectMap FindAndReplaceObjects(string query, string replace)
	{
		array<ref EditorObjectData> created_data = {};
		EditorObjectMap found_objects = FindObjects(query);
		foreach (int _, EditorObject editor_object: found_objects) {
			created_data.Insert(EditorObjectData.Create(replace, editor_object.GetData().Position, editor_object.GetData().Orientation, editor_object.GetData().Scale, editor_object.GetData().Flags));
		}
		
		GetEditor().DeleteObjects(found_objects);
		
		return GetEditor().CreateObjects(created_data);
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_CMD_FIND_AND_REPLACE";
	}
}