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
		
		array<Object> objects = {};
		if (settings.ExportEntireMap) {	
			GetGame().GetObjectsAtPosition3D(vector.Zero, 100000, objects, null);
		} else {
			foreach (EditorObjectData editor_object: data.EditorObjects) {
				objects.Insert(editor_object.WorldObject);
			}
		}
		
		foreach (Object world_object: objects) {
			if (world_object.GetType() == string.Empty) {
				continue;
			}
			
			if (!world_object.IsKindOf("BoundingBoxBase")) {
				continue;
			}
			
			vector orientation = world_object.GetOrientation();
			vector rpy = Vector(orientation[2], orientation[1], orientation[0]);
			float a;
			if (rpy[2] <= -90) {
				a = -rpy[2] - 270;
			} else {
				a = 90 - rpy[2];
			}
			
			FPrintln(handle, string.Format("	<group name=\"%1\" pos=\"%2\" rpy=\"%3\" a=\"%4\"/>", world_object.GetType(), world_object.GetPosition().ToString(false), rpy.ToString(false), a));		 //a=\"%4\"
		}
		
		FPrintln(handle, "</map>");
		
		CloseFile(handle);
	}
	
	override string GetExtension() 
	{
		return ".xml";
	}
}