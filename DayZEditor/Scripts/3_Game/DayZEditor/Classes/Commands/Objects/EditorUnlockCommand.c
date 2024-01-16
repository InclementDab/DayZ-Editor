class EditorUnlockCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		array<EditorObject> selected_objects = EditorObject.GetSelectedEditorObjects();
		foreach (EditorObject selected_object: selected_objects) {		
			GetEditor().UnlockObject(selected_object);
		}
		
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_UNLOCK";
	}
}