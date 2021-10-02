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
			vector rpy = Vector(editor_object.Orientation[2], editor_object.Orientation[1], editor_object.Orientation[0]);
			float a;
			if (rpy[2] <= -90) {
				a = -rpy[2] - 270;
			} else {
				a = 90 - rpy[2];
			}
			
			FPrintln(handle, string.Format("	<group name=\"%1\" pos=\"%2\" rpy=\"%3\" a=\"%4\"/>", editor_object.Type, editor_object.Position.ToString(false), rpy.ToString(false), a));		 //a=\"%4\"
		}
		
		FPrintln(handle, "</map>");
		
		CloseFile(handle);
	}
	
	override string GetExtension() 
	{
		return ".xml";
	}
}