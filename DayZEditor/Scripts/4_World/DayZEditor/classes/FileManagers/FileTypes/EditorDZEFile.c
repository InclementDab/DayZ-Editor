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
			
			EditorObjectData dta = EditorObjectData.Create(object_data.Type, object_data.Position, object_data.Orientation, object_data.Scale, object_data.Flags);
			
			
			bug_fix_save_data.EditorObjects.Insert(dta);
		}
			
		foreach (int id, EditorDeletedObjectData deleted_object: save_data.EditorHiddenObjects) {
			EditorDeletedObjectData deleted_dta = EditorDeletedObjectData.Create(deleted_object.Type, deleted_object.Position);
			deleted_dta.Flags = deleted_object.Flags;
			bug_fix_save_data.EditorHiddenObjects.Insert(deleted_dta);
		}
				
		bug_fix_save_data.MapName = save_data.MapName;
		bug_fix_save_data.CameraPosition = save_data.CameraPosition;
		return bug_fix_save_data;
	}
	
	static string LoadMapName(string file)
	{
		if (FileExist(file)) {
			string bincheck;
			FileSerializer serializer = new FileSerializer();
			serializer.Open(file);
			
			serializer.Read(bincheck);
			if (bincheck != EditorSaveData.BIN_CHECK) {
				return string.Empty;
			}
			
			int read_version;
			serializer.Read(read_version);
			
			string map_name;
			serializer.Read(map_name);
			
			return map_name;
		}
		
		return string.Empty;
	}
	
	override EditorSaveData Import(string file, ImportSettings settings)
	{
		EditorSaveData save_data = new EditorSaveData();
		
		if (!FileExist(file)) {
			EditorLog.Error("File not found %1", file);
			return save_data;
		}
		
		if (EditorSaveData.IsBinnedFile(file)) {
			save_data = LoadBinFile(file);
		} else {
			save_data = LoadJsonFile(file);
		}
		
		foreach (EditorObjectData object_data: save_data.EditorObjects) {
			if (object_data.Type.Contains(".p3d")) {
				vector center = GetP3dBoundingCenter(object_data.Type);
				object_data.Position = object_data.Position + center;
				object_data.Orientation = object_data.Orientation * Math.RAD2DEG;
			}
		}
		
						
		return save_data;
	}
	
	override void Export(EditorSaveData data, string file, ExportSettings settings, eDialogExtraSetting dialog_setting)
	{		
		if (FileExist(file) && !DeleteFile(file)) {
			return;
		}
		
		FileSerializer file_serializer = new FileSerializer();
		if (!file_serializer.Open(file, FileMode.WRITE)) {
			EditorLog.Error("Failed to open file %1", file);
			return;
		}
		
		// sigh
		foreach (EditorObjectData object_data: data.EditorObjects) {
			if (object_data.Type.Contains(".p3d")) {
				vector center = GetP3dBoundingCenter(object_data.Type);
				object_data.Position = object_data.Position - center;
				object_data.Orientation = object_data.Orientation * Math.DEG2RAD;
			}
		}	
		
		data.Write(file_serializer, EditorSaveData.Version);
		file_serializer.Close();
		
		// Undo the crap you just had to do
		foreach (EditorObjectData object_data2: data.EditorObjects) {
			if (object_data2.Type.Contains(".p3d")) {
				object_data2.Position = object_data2.Position + GetP3dBoundingCenter(object_data2.Type);
				object_data2.Orientation = object_data2.Orientation * Math.RAD2DEG;
			}
		}
	}
	
	override string GetExtension() 
	{
		return ".dze";
	}

	override void GetValidExtensions(notnull inout array<ref Param2<string, string>> valid_extensions)
	{
		super.GetValidExtensions(valid_extensions);
		valid_extensions.Insert(new Param2<string, string>("DayZ Editor", "*.dze"));
	}

	override bool CanDoDeletion()
	{
		return true;
	}
};