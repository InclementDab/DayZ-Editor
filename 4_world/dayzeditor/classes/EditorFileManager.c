
class EditorWorldObject
{
	string Classname;
	vector Transform[4];
	
	void EditorWorldObject(string classname, vector transform[4]) 
	{
		Classname = classname; Transform = transform;
	}
}

class EditorWorldData
{
	string MapName;
	vector CameraPosition[4];
	ref array<ref EditorWorldObject> WorldObjects;
	
	void EditorWorldData()
	{
		WorldObjects = new array<ref EditorWorldObject>();
	}
	

	
}

enum ExportMode 
{
	TERRAINBUILDER,
	COMFILE, 
	EXPANSION
}


enum HeightType 
{
	ASL,
	ATL
}

class ExportSettings
{
	static HeightType ExportHeightType;
}

class EditorFileManager
{

	static void SaveFile(EditorWorldData data, string filename = "$profile:editor_save.txt")
	{
		JsonFileLoader<EditorWorldData>.JsonSaveFile(filename, data);
	}
	
	static EditorWorldData LoadFile(string filename = "$profile:editor_save.txt")
	{
		EditorWorldData data = new EditorWorldData();
		JsonFileLoader<EditorWorldData>.JsonLoadFile(filename, data);
		
		return data;
		
	}
	
	static void ExportToFile(ExportMode mode = ExportMode.TERRAINBUILDER, string filename = "$profile:editor_export.txt")
	{
		DeleteFile(filename);
		FileHandle handle = OpenFile(filename, FileMode.WRITE | FileMode.APPEND);
		if (handle == 0) {
			Print("ExportToFile Failed: 0");
			return;
		}
		
		foreach (EditorObject editor_object: Editor.PlacedObjects) {
						
			vector position = editor_object.GetObject().GetPosition();
			vector orientation = editor_object.GetObject().GetOrientation();
			string line;
			
			vector terrainbuilder_offset = Vector(200000, 0, 0);
			
			switch (mode) {
				
				case (ExportMode.TERRAINBUILDER): {
					// "construction_house2";206638.935547;6076.024414;146.000015;0.000000;0.000000;1.000000;
					position[1] = GetGame().SurfaceY(position[0], position[2]) - position[1];
					line = string.Format("\"%1\";%2;%4;%3;%6;%5;%7;", editor_object.GetModelName(), position[0] + terrainbuilder_offset[0], position[1] + terrainbuilder_offset[1], position[2] + terrainbuilder_offset[2], orientation[0], orientation[1], orientation[2]);
					FPrintln(handle, line);
					break;
				}
					
				case (ExportMode.COMFILE): {
					// SpawnObject("Land_Construction_House2", "6638.935547 7.190318 6076.024414", "146.000015 0.000000 0.000000")
					line = string.Format("SpawnObject(\"%1\", \"%2\", \"%3\");", editor_object.GetType(), position.ToString(false), orientation.ToString(false));
					FPrintln(handle, line);
					break;
				}
				
				case ExportMode.EXPANSION: {
					// Land_Construction_House2|13108.842773 10.015385 6931.083984|-101.999985 0.000000 0.000000
					//orientation = orientation.VectorToAngles();
					line = string.Format("%1|%2 %3 %4|%5 %6 %7", editor_object.GetType(), position[0], position[1], position[2], orientation[0], orientation[1], orientation[2]);
					FPrintln(handle, line);
					break;
				}
				
				default: {
					FPrintln(handle, "Line Export Failure");
					break;
				}
				
				
				
			} 
		}
		CloseFile(handle);
	}
}