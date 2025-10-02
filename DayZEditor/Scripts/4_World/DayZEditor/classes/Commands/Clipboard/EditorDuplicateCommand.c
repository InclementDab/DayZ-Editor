class EditorDuplicateCommand: EditorCommand
{	
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		EditorObjectMap selected_objects = GetEditor().GetSelectedObjects();
		array<ref EditorObjectData> created_data = {};
				
		foreach (int i, EditorObject editor_object: selected_objects) {
			created_data.Insert(EditorObjectData.Create(editor_object.GetType(), editor_object.GetPosition(), editor_object.GetOrientation(), editor_object.GetScale(), editor_object.GetFlags()));
		}
		
		GetEditor().ClearSelection();
		EditorObjectMap created_objects = GetEditor().CreateObjects(created_data);
		GetEditor().SelectObjects(created_objects);
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

	override Symbols GetSymbol()
	{
		return Symbols.CLONE;
	}
	
	override bool CanExecute()
	{
		return GetEditor().GetSelectedObjects().Count() > 0;
	}
}