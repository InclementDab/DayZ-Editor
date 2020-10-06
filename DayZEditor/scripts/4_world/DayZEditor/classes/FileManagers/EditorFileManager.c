enum ExportMode 
{
	TERRAINBUILDER,
	EXPANSION,
	COMFILE,
	VPP
}

enum ImportMode
{
	TERRAINBUILDER,
	EXPANSION, 
	COMFILE,
	VPP
}

enum HeightType 
{
	ABSOLUTE,
	RELATIVE
}

class ExportSettings
{
	ExportMode ExportFileMode;
	HeightType ExportHeightType;
	bool ExportSelectedOnly;
	vector ExportOffset;
	string ExportSetName;
}

enum FileDialogResult
{
	SUCCESS = 0,
	NOT_FOUND = 1,
	IN_USE = 2,
	NOT_SUPPORTED = 3,
	UNKNOWN_ERROR = 100
}


class EditorSaveData
{
	bool Binarized = 1;
	string MapName = "ChernarusPlus";
	vector CameraPosition[4];
	ref EditorObjectDataSet EditorObjects = new EditorObjectDataSet();
	
	static EditorSaveData CreateFromEditor(Editor editor)
	{
		EditorSaveData save_data = new EditorSaveData();
		
		// Save world name
		save_data.MapName = GetGame().GetWorldName();
		
		// Save Camera Position
		editor.GetCamera().GetTransform(save_data.CameraPosition);
		
		// Save Objects
		EditorObjectSet placed_objects = editor.GetPlacedObjects();
		if (placed_objects) {
			foreach (EditorObject editor_object: placed_objects) {
				save_data.EditorObjects.InsertEditorData(editor_object.GetData());
			}
		}
		
		return save_data;
	}
}


class EditorFileManager
{
	static void GetSafeFileName(string file_name, string extension)
	{
		TStringArray file_split = {};
		file_name.Split(".", file_split);
		if (file_split.Count() == 1 || file_split[1] != extension) {
			file_name = file_split[0] + extension;
		}
	}
	

	static FileDialogResult Save(ref EditorSaveData data, string file_name)
	{		
		GetSafeFileName(file_name, ".dze");
		
		if (FileExist(file_name) && !DeleteFile(file_name)) {
			return FileDialogResult.IN_USE;
		}
		
		Cerealizer file_serializer = new Cerealizer();
		if (!file_serializer.Open(file_name, FileMode.WRITE)) {
			return FileDialogResult.IN_USE;
		}
		
		if (!file_serializer.Write(data)) {
			file_serializer.Close();
			return FileDialogResult.UNKNOWN_ERROR;
		}
		
		file_serializer.Close();
		
		return FileDialogResult.SUCCESS;
	}
	
	static FileDialogResult Open(out EditorSaveData data, string file_name)
	{
		GetSafeFileName(file_name, ".dze");
		
		Cerealizer file_serializer = new Cerealizer();

		if (!FileExist(file_name)) {
			return FileDialogResult.NOT_FOUND;
		}
		
		if (!file_serializer.Open(file_name, FileMode.READ)) {
			return FileDialogResult.IN_USE;
		}
		
		if (!file_serializer.Read(data)) {
			file_serializer.Close();
			return FileDialogResult.UNKNOWN_ERROR;
		}
		
		file_serializer.Close();
		
		return FileDialogResult.SUCCESS;
	}
	
	static FileDialogResult Import(out ref EditorSaveData data, string file_name, ImportMode mode)
	{		
		if (!FileExist(file_name)) {
			return FileDialogResult.NOT_FOUND;
		}
		
		switch (mode) {
			
			case (ImportMode.COMFILE): {
				
				COMImportData com_data = new COMImportData();
				JsonFileLoader<COMImportData>.JsonLoadFile(file_name, com_data);
				
				foreach (ref Param3<string, vector, vector> param: com_data.m_SceneObjects) {
					EditorLog.Debug("ImportFromFile::COMFILE::Import " + param.param1);					
					data.EditorObjects.InsertEditorData(EditorObjectData.Create(param.param1, param.param2, param.param3));
				}
				
				break;
			}
			
			case (ImportMode.EXPANSION): {
				EditorLog.Debug("EditorFileManager::Import::EXPANSION");
				ExpansionImportData.ReadFromFile(data.EditorObjects, file_name);
				break;
			}
			
			case (ImportMode.VPP): {
				EditorLog.Debug("EditorFileManager::Import::VPP");
				return ImportVPPData(data.EditorObjects, file_name);
			}
			
			default: {
				
				EditorLog.Debug(string.Format("%1 not implemented!", typename.EnumToString(ImportMode, mode)));
				break;
			}
		}

		
		return FileDialogResult.SUCCESS;
		
	}
	
	static FileDialogResult Export(EditorSaveData data, string file_name, ExportSettings export_settings)
	{
		Print("EditorFileManager::Export");		
		DeleteFile(file_name);
		
		switch (export_settings.ExportFileMode) {
			
			case ExportMode.TERRAINBUILDER: {
				return ExportTBData(data.EditorObjects, file_name, export_settings);
			}
			
			case ExportMode.EXPANSION: {
				return ExportExpansionData(data.EditorObjects, file_name);
			}			
			
			case ExportMode.COMFILE: {
				return ExportCOMData(data.EditorObjects, file_name);
			}			
			
			case ExportMode.VPP: {
				return ExportVPPData(data.EditorObjects, file_name);
			}
			
			default: {
				EditorLog.Error("ExportMode not supported! " + export_settings.ExportFileMode);
				return FileDialogResult.NOT_SUPPORTED;
			}
		}
		
		return FileDialogResult.NOT_SUPPORTED;		
	}
}