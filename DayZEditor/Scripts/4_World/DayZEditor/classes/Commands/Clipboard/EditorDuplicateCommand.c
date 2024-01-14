class EditorDuplicateCommand: EditorCommand
{	
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		array<EditorObject> selected_objects = EditorObject.SelectedObjects;
		array<Object> objects = {};
				
		foreach (EditorObject editor_object: selected_objects) {
			objects.Insert(editor_object.GetWorldObject());
		}
		
		array<EditorObject> created_objects = GetEditor().CreateObjects(objects);
		
		EditorObject.ClearSelections();
		foreach (EditorObject created_object: created_objects) {
			if (created_object) {
				created_object.SetSelected(true);
			}
		}
		
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_DUPLICATE";
	}
}