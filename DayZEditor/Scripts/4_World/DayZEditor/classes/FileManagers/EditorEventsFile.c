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
		
		FPrintln(handle, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
		FPrintln(handle, "<events>");
		
		foreach (EditorObjectData editor_object: data.EditorObjects) {
			FPrintln(handle, string.Format("	<event name=\"%1\">", editor_object.Type));			
			FPrintln(handle, string.Format("		<pos x=\"%1\" y=\"%2\" z=\"%3\" a\"%4\"/>", editor_object.Position[0], editor_object.Position[1], editor_object.Position[2], editor_object.Orientation[1]));
			FPrintln(handle, string.Format("	</event>"));
		}
		
		FPrintln(handle, "</events>");
		
		CloseFile(handle);
	}
}