class EditorInitFile: EditorFileType
{
	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
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