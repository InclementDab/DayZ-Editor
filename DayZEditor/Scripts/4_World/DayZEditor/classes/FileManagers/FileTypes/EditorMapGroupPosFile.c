class EditorMapGroupPosFile: EditorFileType
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
		FPrintln(handle, "<map>");
		
		foreach (EditorObjectData editor_object: data.EditorObjects) {
			float a;
			if (editor_object.Orientation[2] <= -90) {
				a = -editor_object.Orientation[2] - 270;
			} else {
				a = 90 - editor_object.Orientation[2];
			}
			
			FPrintln(handle, string.Format("	<group name=\"%1\" pos=\"%2\" rpy=\"%3\"/>", editor_object.Type, editor_object.Position.ToString(false), editor_object.Orientation.ToString(false), a));		 //a=\"%4\"
		}
		
		FPrintln(handle, "</map>");
		
		CloseFile(handle);
	}
	
	override string GetExtension() 
	{
		return ".xml";
	}
}