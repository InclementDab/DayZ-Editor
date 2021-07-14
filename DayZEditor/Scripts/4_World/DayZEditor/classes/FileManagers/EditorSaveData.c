class EditorSaveData
{
	string MapName = "ChernarusPlus";
	vector CameraPosition;
	
	// Do NOT use a map here. ID is stored in EditorObjectData anyway
	ref array<ref EditorObjectData> EditorObjects = {};
		
	ref array<ref EditorDeletedObjectData> EditorDeletedObjects = {};	
		
	void ~EditorSaveData()
	{
		delete EditorObjects;
		delete EditorDeletedObjects;
	}
	
	static EditorSaveData CreateFromEditor(Editor editor, bool selected_only = false)
	{
		EditorSaveData save_data = new EditorSaveData();
		
		// Save world name
		save_data.MapName = GetGame().GetWorldName();
		
		// Save Camera Position
		save_data.CameraPosition = editor.GetCamera().GetPosition();
		
		// Save Objects
		EditorObjectMap placed_objects = editor.GetPlacedObjects();
		if (selected_only) {
			placed_objects = editor.GetSelectedObjects();
		}
		
		if (placed_objects) {
			foreach (EditorObject editor_object: placed_objects) {
				if (editor_object.GetType() != string.Empty) {
					save_data.EditorObjects.Insert(editor_object.GetData());
				}
			}
		}
		
		EditorDeletedObjectMap deleted_objects = editor.GetObjectManager().GetDeletedObjects();
		foreach (int id, EditorDeletedObject deleted_object: deleted_objects) {
			save_data.EditorDeletedObjects.Insert(deleted_object.GetData());
		}
		
		return save_data;
	}
}