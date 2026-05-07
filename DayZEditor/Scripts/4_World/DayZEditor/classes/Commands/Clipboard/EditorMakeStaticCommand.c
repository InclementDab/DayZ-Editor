[RegisterEditorCommand(EditorMakeStaticCommand)]
class EditorMakeStaticCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		array<EditorObject> selected_objects = GetEditor().GetSelectedObjectsOrdered();
		array<ref EditorObjectData> created_data = {};
				
		for (int i = selected_objects.Count() - 1; i >= 0; --i) {			
			created_data.Insert(EditorObjectData.Create(selected_objects[i].GetWorldObject().GetShapeName(), selected_objects[i].GetPosition(), selected_objects[i].GetOrientation(), selected_objects[i].GetScale(), selected_objects[i].GetFlags()));
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
		array<EditorObject> selected_objects = GetEditor().GetSelectedObjectsOrdered();
		array<ref EditorObjectData> created_data = {};
				
		for (int i = selected_objects.Count() - 1; i >= 0; --i) {
			string object_type = GetEditor().GetObjectManager().ConvertP3dFileToPotentialObjectType(selected_objects[i].GetWorldObject().GetShapeName());
			if (!object_type) {
				selected_objects.Remove(i);
				continue;
			}
			
			created_data.Insert(EditorObjectData.Create(object_type, selected_objects[i].GetPosition(), selected_objects[i].GetOrientation(), selected_objects[i].GetScale(), selected_objects[i].GetFlags()));
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