[RegisterEditorCommand(EditorMakeStaticCommand)]
class EditorMakeStaticCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		EditorObjectMap selected_objects = GetEditor().GetSelectedObjects();
		array<ref EditorObjectData> created_data = {};
				
		foreach (int i, EditorObject editor_object: selected_objects) {
			if (!editor_object.IsStatic()) {
				created_data.Insert(EditorObjectData.Create(editor_object.GetWorldObject().GetShapeName(), editor_object.GetPosition(), editor_object.GetOrientation(), editor_object.GetScale(), editor_object.GetFlags()));
			} else {
				//GetEditor().DeselectObject(editor_object);
				selected_objects.Remove(i);
			}
		}
		
		GetEditor().DeleteObjects(selected_objects);
		
		auto created = GetEditor().CreateObjects(created_data);
		GetEditor().SelectObjects(created);
		return true;
	}
	
	override string GetName() 
	{
		return "Convert To Static";
	}
		
	override bool CanExecute()
	{
		return GetEditor().GetSelectedObjects().Count() > 0;
	}
}

[RegisterEditorCommand(EditorMakeConfigCommand)]
class EditorMakeConfigCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		EditorObjectMap selected_objects = GetEditor().GetSelectedObjects();
		array<ref EditorObjectData> created_data = {};
				
		foreach (int i, EditorObject editor_object: selected_objects) {
			if (editor_object.IsStatic()) {
				string object_type = GetEditor().GetObjectManager().ConvertP3dFileToPotentialObjectType(editor_object.GetWorldObject().GetShapeName());
				if (object_type) {
					created_data.Insert(EditorObjectData.Create(object_type, editor_object.GetPosition(), editor_object.GetOrientation(), editor_object.GetScale(), editor_object.GetFlags()));
				}
			} else {
				selected_objects.Remove(i);
				//GetEditor().DeselectObject(editor_object);
			}
		}
		
		GetEditor().DeleteObjects(selected_objects);
		
		auto created = GetEditor().CreateObjects(created_data);
		GetEditor().SelectObjects(created);
		
		return true;
	}
	
	override string GetName() 
	{
		return "Convert To Config";
	}
		
	override bool CanExecute()
	{
		return GetEditor().GetSelectedObjects().Count() > 0;
	}
}