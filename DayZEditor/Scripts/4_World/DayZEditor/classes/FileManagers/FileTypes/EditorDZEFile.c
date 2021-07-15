class EditorDZEFile: EditorFileType
{
	EditorSaveData LoadBinFile(string file)
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
		
		if (!save_data.Read(file_serializer)) {
			file_serializer.Close();
			EditorLog.Error("Could not read file %1", file);
			return save_data;
		}
		
		file_serializer.Close();
		
		return save_data;
	}
	
	EditorSaveData LoadJsonFile(string file)
	{
		EditorSaveData save_data = new EditorSaveData();
		EditorJsonLoader<EditorSaveData>.LoadFromFile(file, save_data);
		
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
		return save_data;
	}
	
	override EditorSaveData Import(string file, ImportSettings settings)
	{
		EditorSaveData save_data = new EditorSaveData();
		
		if (!FileExist(file)) {
			EditorLog.Error("File not found %1", file);
			return save_data;
		}
		
		if (file.Contains("dzebin")) {
			save_data = LoadBinFile(file);
		} else {
			save_data = LoadJsonFile(file);
		}
		
		return save_data;
	}
	
	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
		if (FileExist(file) && !DeleteFile(file)) {
			return;
		}

		if (settings.Binarized) {
			FileSerializer file_serializer = new FileSerializer();
			if (!file_serializer.Open(file, FileMode.WRITE)) {
				EditorLog.Error("Failed to open file %1", file);
				return;
			}
			
			data.Write(file_serializer);
			file_serializer.Close();
		} else {
			EditorJsonLoader<EditorSaveData>.SaveToFile(file, data);
		}
	}
	
	override string GetExtension() 
	{
		return ".dze";
	}
}