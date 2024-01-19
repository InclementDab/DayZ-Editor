class EditorInitFile: EditorFileType
{
	static const int FILE_READ_SIZE = 2048;
	
	override EditorSaveData Import(string file, ImportSettings settings)
	{
		EditorLog.Trace("EditorInitFile::Import");
		
		FileHandle handle = OpenFile(file, FileMode.READ);
		if (!handle) {
			EditorLog.Error("File in use %1", file);
			return null;
		}
		
		EditorSaveData save_data = new EditorSaveData();
		string file_contents;
		int read_size = FILE_READ_SIZE;
		while (read_size > 0) {
			string read_contents;
			read_size = ReadFile(handle, read_contents, FILE_READ_SIZE);
			file_contents += read_contents;
		}
		
		array<string> file_contents_split = {};
		file_contents.Split("\n", file_contents_split);
		foreach (string line: file_contents_split) {

			// Bit of a hacky way of doing this
			// Other idea is to actually run the 'main' script and then enumerate all the spawned objects,
			// then add them to the Editor
			line.TrimInPlace();
			if (line.Contains("SpawnObject") && !line.Contains("void") && !line.Contains("static")) {
				
				TStringArray tokens = {};
		        line.Split("\"", tokens);

				if (line.Length() < 5) {
					EditorLog.Error("Invalid SpawnObject data found! %1", line);
				}
				
	        	/*
				tokens[1]; // Building
	        	tokens[3]; // Position
	        	tokens[5]; // Orientation
				tokens[7]; // Scale
	        	*/
				
				float scale = 1.0;
				if (tokens.Count() > 6) {
					scale = tokens[7].ToFloat();
				}
								
				save_data.EditorObjects.Insert(EditorObjectData.Create(tokens[1], tokens[3].ToVector(), tokens[5].ToVector(), 1, EFE_DEFAULT));
			}
		}        

		CloseFile(handle);
		
		return save_data;
	}
	
	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
		EditorLog.Trace("EditorInitFile::Export");
		
		if (!CopyFile("Editor/scripts/data/Defaults/init.c", file)) {
			EditorLog.Error("Failed to copy file %1", file);
			return;
		}
		
		FileHandle handle = OpenFile(file, FileMode.APPEND);
		if (!handle) {
			return;
		}
		
		TStringArray spawn_method = {};
		spawn_method.Insert("\n\n\/\/ Paste anything below this line into the bottom of your 'void main()' function");
		
		if (data.EditorObjects.Count() > 0) {
			spawn_method.Insert("\n\n\/\/ Created Objects");
		}
		foreach (EditorObjectData editor_object: data.EditorObjects) {
			spawn_method.Insert(string.Format("SpawnObject(\"%1\", \"%2\", \"%3\", %4);", editor_object.Type, editor_object.Position.ToString(false), editor_object.Orientation.ToString(false), editor_object.Scale));
		}
		
		spawn_method.Insert("\n\n\/\/ Uncomment if you want to export loot from newly added buildings");
		spawn_method.Insert("\/\/ Position, Radius (increase if you have a larger map than Chernarus)");
    	spawn_method.Insert("\/\/ GetCEApi().ExportProxyData(Vector(7500, GetGame().SurfaceY(7500, 7500), 7500), 20000);");
			
				
		foreach (string line: spawn_method) {
			FPrintln(handle, line);	
		}
				
		CloseFile(handle);
	}
	
	override string GetExtension() 
	{
		return ".c";
	}
}
