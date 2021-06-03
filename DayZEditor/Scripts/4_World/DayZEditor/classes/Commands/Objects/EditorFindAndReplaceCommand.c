class EditorFindAndReplaceCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		EditorFindAndReplaceDialog dialog("Find and Replace");
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
			if (t1 == query) {
				found_objects.InsertEditorObject(editor_object);
			}
		}
		
		return found_objects;
	}
	
	//todo maybe have action for Replace, and create a replace lambda
	EditorObjectMap FindAndReplaceObjects(string query, string replace)
	{
		EditorObjectDataMap replaced_objects();
		EditorObjectMap found_objects = FindObjects(query);
		foreach (int _, EditorObject editor_object: found_objects) {
			replaced_objects.InsertData(EditorObjectData.Create(replace, editor_object.GetData().Position, editor_object.GetData().Orientation, editor_object.GetData().Scale, editor_object.GetData().Flags));
		}
		
		GetEditor().DeleteObjects(found_objects);
		
		return GetEditor().CreateObjects(replaced_objects);
	}
	
	override string GetName()
	{
		return "Find and Replace";
	}
}