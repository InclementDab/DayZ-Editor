class EditorCheckDuplicatesCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		EditorObjectMap duplicate_objects();
		
		// prioritize selected objects first, else use all placed objects
		EditorObjectMap editor_objects = GetEditor().GetSelectedObjects();
		if (editor_objects.Count() == 0) {
			editor_objects = GetEditor().GetPlacedObjects();
		}
		
		foreach (int id, EditorObject editor_object: editor_objects) {
			Object world_object = editor_object.GetWorldObject();
			array<Object> world_objects = {};
			array<CargoBase> cargo_base = {};
			GetGame().GetObjectsAtPosition3D(world_object.GetPosition(), 1, world_objects, cargo_base);
			
			if (duplicate_objects[id]) {
				continue;
			}
			
			foreach (Object found_object: world_objects) {
				if (found_object == world_object) {
					continue;
				}
				
				if (found_object.GetType() != world_object.GetType()) {
					continue;
				}
				
				duplicate_objects.InsertEditorObject(GetEditor().GetEditorObject(found_object));
			}
		}
		
		if (duplicate_objects.Count() == 0) {
			EditorMessageBox.Show(GetName(), "No duplicate objects found in search", MessageBoxButtons.OK);
			EditorLog.Info("No duplicate objects found!");
			return;
		}
		
		EditorLog.Info("%1 duplicate objects found in search", duplicate_objects.Count().ToString());
		
		DialogResult result = EditorMessageBox.Show(GetName(), string.Format("%1 duplicate objects found in search, delete?", duplicate_objects.Count()), MessageBoxButtons.YesNo);
		if (result != DialogResult.Yes) {
			return;
		}
		
		GetEditor().DeleteObjects(duplicate_objects);
	}
	
	override string GetName() 
	{
		return "Check for Duplicates";
	}
}