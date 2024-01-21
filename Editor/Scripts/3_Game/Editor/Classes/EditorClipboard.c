class EditorClipboard
{
	static void Cut()
	{
		EditorLog.Trace("EditorObjectManager::CutSelection");
		
		array<EditorObject> cut_objects = EditorObject.GetSelectedEditorObjects();
		if (cut_objects.Count() == 0) {
			return;
		}
		
		Copy();
		
		GetDayZGame().GetEditor().DeleteObjects(cut_objects);
	}
	
	static void Copy()
	{
		EditorLog.Trace("EditorObjectManager::CopySelection");
		array<EditorObject> copy_objects = EditorObject.GetSelectedEditorObjects();
		if (copy_objects.Count() == 0) {
			return;
		}
		
		array<ref EditorObjectData> world_objects = {};
		
		vector avg_position;
		foreach (int id, EditorObject copy_object: copy_objects) {
			avg_position += copy_object.GetWorldObject().GetPosition();
		}
		
		for (int i = 0; i < 3; i++) {
			avg_position[i] = avg_position[i] / copy_objects.Count();
		}
		
		avg_position[1] = GetGame().SurfaceY(avg_position[0], avg_position[2]);
		
		foreach (EditorObject selected_object: copy_objects) {
 			//EditorObjectData data = selected_object.CreateSerializedData();
			//data.Position = selected_object.GetWorldObject().GetPosition() - avg_position;
			//world_objects.Insert(data);
		}
	
		string clipboard_data = JsonFileLoader<array<ref EditorObjectData>>.JsonMakeData(world_objects);		
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

		EditorObject.ClearSelections();
		
		array<Object> created_objects = {};
		foreach (EditorObjectData pasted_object: data) {			
			
			EditorObjectData dta = EditorObjectData.Create(pasted_object.Type, pasted_object.Position + GetDayZGame().GetEditor().GetCamera().PerformCursorRaycast().Position, pasted_object.Orientation, pasted_object.Scale, pasted_object.Flags);
			created_objects.Insert(dta.CreateObject());
		}	
		
		/*
		array<EditorObject> editor_objects = GetDayZGame().GetEditor().CreateObjects(created_objects);
		
		foreach (EditorObject editor_object: editor_objects) {
			vector position = editor_object.GetWorldObject().GetPosition();
			vector transform[4] = {
				"1 0 0",
				"0 1 0",
				"0 0 1",
				position
			};
			
			if (GetDayZGame().GetEditor().MagnetMode) {
				set<Object> o;
				vector ground, ground_dir; int component;
				DayZPhysics.RaycastRV(position, position + transform[1] * -1000, ground, ground_dir, component, o, NULL, editor_object.GetWorldObject(), false, true); // set to ground only
				vector surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
				
				// BREAK STUFF I DARE YOU >:(
				EntityAI.Cast(editor_object.GetWorldObject()).PlaceOnSurfaceRotated(transform, position, surface_normal[0] * -1, surface_normal[2] * -1, 0, GetDayZGame().GetEditor().MagnetMode);
				transform[3] = transform[3] - transform[1] * vector.Distance(ground, position);
				editor_object.GetWorldObject().SetTransform(transform);				
			}
			
			editor_object.SetSelected(true);
		}*/
	}
}