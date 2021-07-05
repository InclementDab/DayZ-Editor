typedef Param2<vector, vector> EventDataPair;
typedef array<ref EventDataPair> EventDataArray;

class EditorEventsFile: EditorFileType
{	
	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
		if (FileExist(file) && !DeleteFile(file)) {
			return;
		}
		
		FileHandle handle = OpenFile(file, FileMode.WRITE);
		if (!handle) {
			EditorLog.Error("File in use %1", file);
			return;
		}
		
		// type
		//			array of position, orientation
		map<string, ref EventDataArray> optimized_positions();
		
		foreach (EditorObjectData editor_object: data.EditorObjects) {
			if (!optimized_positions[editor_object.Type]) {
				optimized_positions[editor_object.Type] = new EventDataArray();
			}
			
			optimized_positions[editor_object.Type].Insert(new EventDataPair(editor_object.Position, editor_object.Orientation));
		}
		
		FPrintln(handle, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
		FPrintln(handle, "<events>");
		
		foreach (string type, ref array<ref EventDataPair> event_datas: optimized_positions) {
			FPrintln(handle, string.Format("	<event name=\"%1\">", type));			
			foreach (EventDataPair event_data: event_datas) {
				FPrintln(handle, string.Format("		<pos x=\"%1\" y=\"%2\" z=\"%3\" a=\"%4\"/>", event_data.param1[0], event_data.param1[1], event_data.param1[2], event_data.param2[1]));
			}
			
			FPrintln(handle, string.Format("	</event>"));
		}
		
		FPrintln(handle, "</events>");
		
		CloseFile(handle);
	}
	
	override string GetExtension() 
	{
		return ".xml";
	}
}