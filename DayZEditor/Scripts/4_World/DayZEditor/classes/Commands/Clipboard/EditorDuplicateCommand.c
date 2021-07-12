class EditorDuplicateCommand: EditorCommand
{	
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		EditorObjectMap selected_objects = GetEditor().GetSelectedObjects();
		EditorObjectDataMap created_data();
				
		foreach (int i, EditorObject editor_object: selected_objects) {
			created_data.InsertData(EditorObjectData.Create(editor_object.GetType(), editor_object.GetPosition(), editor_object.GetOrientation(), editor_object.GetScale(), editor_object.GetFlags()));
		}
		
		EditorObjectMap created_objects = GetEditor().CreateObjects(created_data);
		
		GetEditor().ClearSelection();
		foreach (int id, EditorObject created_object: created_objects) {
			if (created_object) {
				GetEditor().SelectObject(created_object);
			}
		}
		
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_DUPLICATE";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_J };
	}
}