

class EditorExpansionFile: EditorFileType
{
	override EditorFileResult Import(out EditorSaveData data, string file_name, ImportSettings settings) 
	{
		FileHandle handler = OpenFile(file_name, FileMode.READ);
		
		string name;
		vector position, rotation;
		while (GetObjectFromFile(handler, name, position, rotation)) {	
				
			data.EditorObjects.InsertData(EditorObjectData.Create(name, position, rotation, EditorObjectFlags.ALL));
		}
		
		if (handler) {
			CloseFile(handler);
		}
		
		return EditorFileResult.SUCCESS;
	}
	
	override EditorFileResult Export(EditorSaveData data, string file_name, ExportSettings settings)
	{
		FileHandle handle = OpenFile(file_name, FileMode.WRITE);
	
		if (!handle) {
			return EditorFileResult.IN_USE;
		}
		
		foreach (EditorObjectData editor_object: data.EditorObjects) {
			// Land_Construction_House2|13108.842773 10.015385 6931.083984|-101.999985 0.000000 0.000000
			vector position = editor_object.GetPosition();
			vector orientation = editor_object.GetOrientation();
			Print(editor_object.Type);
			string line = string.Format("%1|%2 %3 %4|%5 %6 %7", editor_object.Type, position[0], position[1], position[2], orientation[0], orientation[1], orientation[2]);
			FPrintln(handle, line);
		}
		
		if (handle) {
			CloseFile(handle);
		}
		
		return EditorFileResult.SUCCESS;
	}
	
	
    private static bool GetObjectFromFile(FileHandle file, out string name, out vector position, out vector rotation, out string special = "false")
    {                
        string line;
        int line_size = FGets(file, line);
        
        if (line_size < 1)
            return false;
        
        TStringArray tokens = {};
        line.Split("|", tokens);

        name = tokens.Get(0);        
        position = tokens.Get(1).ToVector();
        rotation = tokens.Get(2).ToVector();    
        special = tokens.Get(3);

        return true;
    }
}
