

class EditorClipboard
{
	
	static void Cut(EditorObjectSet cut_objects)
	{
		EditorLog.Trace("EditorObjectManager::CutSelection");
		Copy(cut_objects);
		GetEditor().DeleteObjects(cut_objects);
	}
	
	
	static void Copy(EditorObjectSet copy_objects)
	{
		EditorLog.Trace("EditorObjectManager::CopySelection");
		
		ref array<ref EditorObjectData>> world_objects = new ref array<ref EditorObjectData>>();
		
		vector avg_position;
		foreach (int id, EditorObject copy_object: copy_objects)
			avg_position += copy_object.GetPosition();
		
		for (int i = 0; i < 3; i++)
			avg_position[i] = avg_position[i] / copy_objects.Count();
		
		foreach (int idx, EditorObject selected_object: copy_objects) {
			EditorObjectData data = selected_object.GetData();
			data.Position = selected_object.GetPosition() - avg_position;
			world_objects.Insert(data);
		}
	
		GetGame().CopyToClipboard(JsonFileLoader<array<ref EditorObjectData>>.JsonMakeData(world_objects));
	}
	
	static void Paste(vector cursor_pos)
	{
		EditorLog.Trace("EditorObjectManager::PasteSelection");
		string clipboard_text;
		GetGame().CopyFromClipboard(clipboard_text);
		
		ref array<ref EditorObjectData>> data = {};
		JsonFileLoader<array<ref EditorObjectData>>.JsonLoadData(clipboard_text, data);
		if (data.Count() == 0) 
			return;
		

		GetEditor().ClearSelection();
				
		foreach (ref EditorObjectData pasted_object: data) {
			
			
			vector position;
			if (GetEditor().GetEditorHud().IsMapVisible()) {
				MapWidget map_widget = GetEditor().GetEditorHud().GetMap();
				int x, y;
				GetMousePos(x, y);
				position = pasted_object.Position + map_widget.ScreenToMap(Vector(x, y, 0));
			} else {
				position = pasted_object.Position + cursor_pos;
			}
			
			vector transform[4] = {
				"1 0 0",
				"0 1 0",
				"0 0 1",
				position
			};
			
			EditorObject editor_object = GetEditor().CreateObject(EditorObjectData.Create(pasted_object.Type, position, pasted_object.Orientation, pasted_object.Flags));
			float surfacey = GetGame().SurfaceY(position[0], position[2]);
			vector size = editor_object.GetSize();
			position[1] = surfacey + size[1] / 2;
			editor_object.SetPosition(position);
			editor_object.Update();
			
			if (GetEditor().GetEditorHud().GetEditorHudController().MagnetButton) {
				
				set<Object> o;
				vector ground, ground_dir; int component;
				DayZPhysics.RaycastRV(position, position + transform[1] * -1000, ground, ground_dir, component, o, NULL, editor_object.GetWorldObject(), false, true); // set to ground only
				vector surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
				editor_object.PlaceOnSurfaceRotated(transform, position, surface_normal[0] * -1, surface_normal[2] * -1, 0, GetEditor().GetEditorHud().GetEditorHudController().MagnetButton);
				transform[3] = transform[3] - transform[1] * vector.Distance(ground, position);
				
				editor_object.SetTransform(transform);
				editor_object.Update();
				
			}
			
			GetEditor().SelectObject(editor_object);
		}	
	}
	
	
	
}

