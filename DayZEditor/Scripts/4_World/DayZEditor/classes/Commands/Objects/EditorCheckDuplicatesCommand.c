class EditorCheckDuplicatesCommand: EditorAsyncCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		array<EditorObject> duplicate_objects = {};
		
		// prioritize selected objects first, else use all placed objects
		array<EditorObject> editor_objects = {};
		if (editor_objects.Count() == 0) {
			array<ref EditorObject> placed = GetEditor().GetPlacedObjects();
			foreach (EditorObject p: placed) {
				editor_objects.Insert(p);
			}
		} else {
			editor_objects = EditorObject.SelectedObjects;
		}
		
		foreach (EditorObject editor_object: editor_objects) {
			Object world_object = editor_object.GetWorldObject();
			array<Object> world_objects = {};
			array<CargoBase> cargo_base = {};
			GetGame().GetObjectsAtPosition3D(world_object.GetPosition(), 1, world_objects, cargo_base);
			
			foreach (Object found_object: world_objects) {
				if (found_object == world_object) {
					continue;
				}
				
				if (found_object.GetType() != world_object.GetType()) {
					continue;
				}
				
				duplicate_objects.Insert(GetEditor().GetEditorObject(found_object));
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
		return "#STR_EDITOR_CMD_CHECK_DUPLICATES";
	}
}