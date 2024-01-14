class EditorClipboard
{
	static void Cut()
	{
		EditorLog.Trace("EditorObjectManager::CutSelection");
		
		array<EditorObject> cut_objects = EditorObject.SelectedObjects;
		if (cut_objects.Count() == 0) {
			return;
		}
		
		Copy();
		GetEditor().DeleteObjects(cut_objects);
	}
	
	static void Copy()
	{
		EditorLog.Trace("EditorObjectManager::CopySelection");
		array<EditorObject> copy_objects = EditorObject.SelectedObjects;
		if (copy_objects.Count() == 0) {
			return;
		}
		
		array<ref EditorObjectData> world_objects = {};
		
		vector avg_position;
		foreach (int id, EditorObject copy_object: copy_objects) {
			avg_position += copy_object.GetPosition();
		}
		
		for (int i = 0; i < 3; i++) {
			avg_position[i] = avg_position[i] / copy_objects.Count();
		}
		
		avg_position[1] = GetGame().SurfaceY(avg_position[0], avg_position[2]);
		
		foreach (int idx, EditorObject selected_object: copy_objects) {
 			EditorObjectData data = EditorObjectData.Create(selected_object.GetType(), selected_object.GetPosition(), selected_object.GetOrientation(), selected_object.GetScale(), selected_object.GetFlags());
			data.Position = selected_object.GetPosition() - avg_position;
			data.Orientation = selected_object.GetOrientation();
			data.Flags = selected_object.GetFlags();
			data.BottomCenter = selected_object.GetBottomCenter();
			world_objects.Insert(data);
		}
	
		string clipboard_data = JsonFileLoader<array<ref EditorObjectData>>.JsonMakeData(world_objects);
		//clipboard_data.Replace("\n", "");
		//clipboard_data.Replace(" ", "");
		
		GetGame().CopyToClipboard(clipboard_data);
	}
	
	static void Paste(vector cursor_pos)
	{
		EditorLog.Trace("EditorObjectManager::PasteSelection");		
		string error, clipboard_text;
		GetGame().CopyFromClipboard(clipboard_text);
		
		JsonSerializer json_serializer = new JsonSerializer();
		array<ref EditorObjectData> data = {};
		if (!json_serializer.ReadFromString(data, clipboard_text, error)) {
			EditorLog.Error("Paste Error: %1", error);
			return;
		}

		GetEditor().ClearSelection();
		
		array<ref EditorObjectData> created_data = {};
		foreach (EditorObjectData pasted_object: data) {
			if (pasted_object.Type == string.Empty) {
				continue;
			}
			
			vector position = pasted_object.Position + Editor.CurrentMousePosition;						
			created_data.Insert(EditorObjectData.Create(pasted_object.Type, position, pasted_object.Orientation, pasted_object.Scale, pasted_object.Flags));
		}	
		
		array<EditorObject> editor_objects = GetEditor().CreateObjects(created_data);
		
		foreach (int id, EditorObject editor_object: editor_objects) {
			//editor_object.SetPosition(position);
			
			vector transform[4] = {
				"1 0 0",
				"0 1 0",
				"0 0 1",
				editor_object.GetPosition()
			};
			
			if (GetEditor().MagnetMode) {
				set<Object> o;
				vector ground, ground_dir; int component;
				DayZPhysics.RaycastRV(position, position + transform[1] * -1000, ground, ground_dir, component, o, NULL, editor_object.GetWorldObject(), false, true); // set to ground only
				vector surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
				editor_object.PlaceOnSurfaceRotated(transform, position, surface_normal[0] * -1, surface_normal[2] * -1, 0, GetEditor().MagnetMode);
				transform[3] = transform[3] - transform[1] * vector.Distance(ground, position);
				editor_object.SetTransform(transform);				
			}
			
			GetEditor().SelectObject(editor_object);
		}
	}
}