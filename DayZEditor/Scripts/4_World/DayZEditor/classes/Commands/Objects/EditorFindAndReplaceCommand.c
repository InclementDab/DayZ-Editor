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
	
	EditorObjectMap FindAndReplaceObjects(string query, string replace)
	{
		query.ToLower();
		replace.ToLower();
		EditorObjectMap replaced_objects();
		EditorObjectMap found_objects = FindObjects(query);
		
		
		return replaced_objects;
	}
	
	override string GetName()
	{
		return "Find and Replace";
	}
}