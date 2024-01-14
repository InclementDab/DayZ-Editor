class EditorShowCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		array<EditorObject> selected_objects = EditorObject.SelectedObjects
		foreach (EditorObject selected_object: selected_objects) {		
			selected_object.Hide(false);
		}
		
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_SHOW";
	}
}