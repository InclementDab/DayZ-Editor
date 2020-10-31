class EditorInitFile: EditorFileType
{
	override EditorSaveData Import(string file, ImportSettings settings)
	{
		EditorLog.Trace("EditorInitFile::Import");
		
		FileHandle handle = OpenFile(file, FileMode.READ);
		if (!handle) {
			EditorLog.Error("File in use %1", file);
			return null;
		}
		
		EditorSaveData save_data = new EditorSaveData();
	
        string line;
        int line_size = FGets(handle, line);
		while (line_size >= 1) {

			// Bit of a hacky way of doing this
			// Other idea is to actually run the 'main' script and then enumerate all the spawned objects,
			// then add them to the Editor
			if (line.Contains("SpawnObject(\"")) {
				TStringArray tokens = {};
		        line.Split("\"", tokens);
	        	/*
				tokens[1]; // Building
	        	tokens[3]; // Position
	        	tokens[5]; // Orientation
	        	*/
				
				save_data.EditorObjects.Insert(EditorObjectData.Create(tokens[1], tokens[3].ToVector(), tokens[5].ToVector(), EditorObjectFlags.ALL));
			}
			
			line_size = FGets(handle, line);
		}        

		CloseFile(handle);
		
		return save_data;
	}
	
	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
		EditorLog.Trace("EditorInitFile::Export");
		
		if (!CopyFile("DayZEditor/scripts/data/Defaults/init.c", file)) {
			EditorLog.Error("Failed to copy file %1", file);
			return;
		}
		
		FileHandle handle = OpenFile(file, FileMode.APPEND);
		if (!handle) {
			return;
		}
		
		TStringArray spawn_method = {
			"\n\nvoid main()",
			"{"
		};
		foreach (EditorObjectData editor_object: data.EditorObjects) {
			// SpawnObject("Land_Construction_House2", "6638.935547 7.190318 6076.024414", "146.000015 0.000000 0.000000")
			spawn_method.Insert(string.Format("	SpawnObject(\"%1\", \"%2\", \"%3\");", editor_object.Type, editor_object.Position.ToString(false), editor_object.Orientation.ToString(false)));
		}
		
		spawn_method.Insert("	\/\/Position, Radius (increase if you have a larger map than Chernarus)");
    	spawn_method.Insert("	GetCEApi().ExportProxyData(Vector(7500, GetGame().SurfaceY(7500, 7500), 7500), 20000);");
			
		spawn_method.Insert("	\n\n\/\/ Deleted Objects");
		
		foreach (int deleted_object: data.DeletedObjects) {
			spawn_method.Insert(string.Format("	DeleteObject(%1);", deleted_object));
		}
			
		spawn_method.Insert("}");
				
		foreach (string line: spawn_method) {
			FPrintln(handle, line);	
		}
				
		CloseFile(handle);
	}
	
	override string GetExtension() {
		return ".c";
	}
}