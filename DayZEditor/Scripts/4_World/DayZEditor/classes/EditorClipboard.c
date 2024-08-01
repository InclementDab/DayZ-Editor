class EditorClipboard
{
	static int Cut(notnull array<EditorObject> editor_objects)
	{
		if (editor_objects.Count() == 0) {
			return 0;
		}
		
		int copy_count = EditorClipboard.Copy(editor_objects);
		GetEditor().DeleteObjects(editor_objects);
		return copy_count;
	}
	
	static int Copy(notnull array<EditorObject> editor_objects)
	{
		if (editor_objects.Count() == 0) {
			return 0;
		}
		
		array<EditorObjectData> world_objects = {};
		foreach (int idx, EditorObject selected_object: editor_objects) {
			if (selected_object && selected_object.GetData()) {
				world_objects.Insert(selected_object.GetData());
			}
		}
	
		string clipboard_data = JsonFileLoader<array<EditorObjectData>>.JsonMakeData(world_objects);
		if (GetEditor().Settings.MinifyCopyData) {
			clipboard_data.Replace("\r", "");
			clipboard_data.Replace("\t", "");
			clipboard_data.Replace("\n", "");
		}
		
		GetGame().CopyToClipboard(clipboard_data);
		return world_objects.Count();
	}
	
	/*
		in: cursor position in world

		out: count of entities pasted
			error: <= -1
	*/
	static int Paste(vector cursor_pos)
	{
		string error, clipboard_text;
		GetGame().CopyFromClipboard(clipboard_text);
		if (!clipboard_text) {
			return -2;
		}
		
		JsonSerializer json_serializer = new JsonSerializer();
		array<ref EditorObjectData> read_data = {};
		if (!json_serializer.ReadFromString(read_data, clipboard_text, error)) {
			return -1;
		}

		if (read_data.Count() == 0) {
			return 0;
		}

		GetEditor().ClearSelection();
		
		vector average_position_table;
		for (int i = 0; i < read_data.Count(); i++) {
			if (read_data[i] && read_data[i].Type) {
				average_position_table = average_position_table + read_data[i].Position;
			}
		}

		average_position_table = average_position_table * (1 / read_data.Count());

		array<ref EditorObjectData> write_data = {};
		for (int j = 0; j < read_data.Count(); j++) {
			if (read_data[j] && read_data[j].Type) {
				vector write_data_position = read_data[j].Position - average_position_table + cursor_pos;
				write_data.Insert(EditorObjectData.Create(read_data[j].Type, write_data_position, read_data[j].Orientation, read_data[j].Scale, read_data[j].Flags));
			}	
		}
		
		EditorObjectMap editor_objects = GetEditor().CreateObjects(write_data);
		foreach (int id, EditorObject editor_object: editor_objects) {						
			if (GetEditor().MagnetMode) {				
				vector transform[4];
				editor_object.GetTransform(transform);
				
				vector ground_transform[4];
				EditorObjectDragHandler.ProjectToGround4(transform, ground_transform);
				editor_object.SetTransform(ground_transform);				
			}
			
			GetEditor().SelectObject(editor_object);
		}

		return editor_objects.Count();
	}
}