class EditorDZEBinFile: EditorFileType
{
	override EditorSaveData Import(string file, ImportSettings settings)
	{
		EditorSaveData save_data = new EditorSaveData();
		
		if (!FileExist(file)) {
			EditorLog.Error("File not found %1", file);
			return save_data;
		}

		FileSerializer file_serializer = new FileSerializer();
		if (!file_serializer.Open(file, FileMode.READ)) {
			EditorLog.Error("File in use %1", file);
			return save_data;
		}
		
		if (!file_serializer.Read(save_data)) {
			file_serializer.Close();
			EditorLog.Error("Could not read file %1", file);
			return save_data;
		}
		
		file_serializer.Close();
		
		// bugfix to fix the id not incrementing
		EditorSaveData bug_fix_save_data = new EditorSaveData();
		foreach (EditorObjectData object_data: save_data.EditorObjects) {
			if (GetGame().GetModelName(object_data.Type) == "UNKNOWN_P3D_FILE") {
				EditorLog.Warning("Ignoring %1 on import. Invalid type, possible for crash", object_data.Type);
			}
			
			bug_fix_save_data.EditorObjects.Insert(EditorObjectData.Create(object_data.Type, object_data.Position, object_data.Orientation, object_data.Scale, object_data.Flags));
		}
			
		foreach (int id, EditorDeletedObjectData deleted_object: save_data.EditorDeletedObjects) {
			bug_fix_save_data.EditorDeletedObjects.Insert(EditorDeletedObjectData.Create(deleted_object.Type, deleted_object.Position));
		}
				
		bug_fix_save_data.MapName = save_data.MapName;
		bug_fix_save_data.CameraPosition = save_data.CameraPosition;
		
		return bug_fix_save_data;
	}
	
	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
		if (FileExist(file) && !DeleteFile(file)) {
			return;
		}	
				
		FileSerializer file_serializer = new FileSerializer();
		if (!file_serializer.Open(file, FileMode.WRITE)) {
			EditorLog.Error("Failed to open file %1", file);
			return;
		}
		
		if (!file_serializer.Write(data)) {
			file_serializer.Close();
			EditorLog.Error("Failed to write to file %1", file);
			return;
		}
		
		file_serializer.Close();
	}
	
	override string GetExtension() 
	{
		return ".dzebin";
	}
}