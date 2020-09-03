

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
	vector ExportOffset;
	ExportMode ExportFileMode;
	HeightType ExportHeightType;
	bool ExportSelectedOnly;
	
	string ExportSetName;
}

typedef ref array<ref EditorObjectData> EditorObjectDataArray;
class EditorWorldData
{
	string MapName;
	vector CameraPosition[4];
	ref EditorObjectDataArray EditorObjects;
	
	void EditorWorldData()
	{
		EditorObjects = new EditorObjectDataArray();
	}
}


enum FileDialogResult
{
	SUCCESS = 0,
	NOT_FOUND = 1,
	IN_USE = 2,
	NOT_SUPPORTED = 3,
	UNKNOWN_ERROR = 100
}

string CreateEditorMission(string map_name = "ChernarusPlus")
{
	Print("CreateEditorMission");
	string mission = "$saves:DayZEditor." + map_name;
	
	if (!FileExist(mission)) {
		Print("Editor Mission not found, creating....");
		MakeDirectory(mission);
	}
	
	FileHandle init = OpenFile(mission + "/init.c", FileMode.WRITE);
	CloseFile(init);
	
	return mission;
}


typedef FileSerializer Cerealizer;


// if you put these methods in a class they go boom
static FileDialogResult VPPFileDataExportData(out ref EditorObjectDataArray data, string file, string set_name)
{
	EditorPrint("VPPFileData::ExportData");
	
	Cerealizer cereal = new Cerealizer();
	
	ref VPPToEditorBuildingSet bSet = new VPPToEditorBuildingSet(set_name);
	
	foreach (EditorObjectData object_data: data) {
		bSet.AddBuilding(object_data.Type, object_data.Position, object_data.Orientation, true);
		bSet.SetActive(true);
	}
	
	
	if (cereal.Open(file, FileMode.APPEND)) {
		cereal.Write(bSet);
		cereal.Close();	
	}
	

	
	return FileDialogResult.SUCCESS;
}

static FileDialogResult VPPFileDataImportData(ref EditorObjectDataArray data, string file)
{
	EditorPrint("VPPFileData::ImportData");

	Cerealizer cereal = new Cerealizer();
	ref VPPToEditorBuildingSet bSet;
	if (cereal.Open(file, FileMode.READ)) {
		cereal.Read(bSet);
		cereal.Close();
		
	} else return FileDialogResult.IN_USE;
	
	if (!bSet) return FileDialogResult.UNKNOWN_ERROR;
	
	ref array<ref VPPToEditorSpawnedBuilding> spawned_buildings = new array<ref VPPToEditorSpawnedBuilding>();
	bSet.GetSpawnedBuildings(spawned_buildings);
	foreach (ref VPPToEditorSpawnedBuilding building: spawned_buildings) {
		string name = building.GetName();
		TStringArray name_split = new TStringArray();
		name.Split("-", name_split);
		data.Insert(EditorObjectData.Create(name_split.Get(0), building.GetPosition(), building.GetOrientation(), EditorObjectFlags.ALL));
	}
	return FileDialogResult.SUCCESS;
}


class ExpansionFileData
{

	static FileDialogResult Import(out ref EditorWorldData data, string file)
	{
		FileHandle handle = OpenFile(file, FileMode.READ);
		if (handle == 0)
			return FileDialogResult.UNKNOWN_ERROR;
		
		string name;
		vector position, rotation;
		while (GetObjectFromFile(handle, name, position, rotation)) {	
			
			string model_name = GetGame().GetModelName(name);
			if (model_name == "UNKNOWN_P3D_FILE") {
				Print(string.Format("%1 is not a valid Object Type!", name));
				continue;
			}
					
			data.EditorObjects.Insert(EditorObjectData.Create(name, position, rotation, EditorObjectFlags.OBJECTMARKER | EditorObjectFlags.LISTITEM));
		}
		
		CloseFile(handle);
		
		return FileDialogResult.SUCCESS;
	}
	
	static FileDialogResult Export(ref EditorWorldData data, string file)
	{
		FileHandle handle = OpenFile(file, FileMode.WRITE);
		if (handle == 0)
			return FileDialogResult.UNKNOWN_ERROR;
		
		// Land_Construction_House2|13108.842773 10.015385 6931.083984|-101.999985 0.000000 0.000000
		//orientation = orientation.VectorToAngles();
		foreach (EditorObjectData editor_object: data.EditorObjects) {
			string line = string.Format("%1|%2 %3 %4|%5 %6 %7", editor_object.Type, editor_object.Position[0], editor_object.Position[1], editor_object.Position[2], editor_object.Orientation[0], editor_object.Orientation[1], editor_object.Orientation[2]);
			FPrintln(handle, line);
		}
		
		return FileDialogResult.SUCCESS;
	}
	
    private static bool GetObjectFromFile(FileHandle file, out string name, out vector position, out vector rotation, out string special = "false")
    {                
        string line;
        int lineSize = FGets(file, line);
        
        if (lineSize < 1)
            return false;
        
        ref TStringArray tokens = new TStringArray;
        line.Split("|", tokens);

        name = tokens.Get(0);        
        position = tokens.Get(1).ToVector();
        rotation = tokens.Get(2).ToVector();    
        special = tokens.Get(3);

        return true;
    }
}

class COMFileData
{
	string name;
	ref array<ref Param3<string, vector, vector>> m_SceneObjects;
	
	void COMImportData()
	{
		m_SceneObjects = new array<ref Param3<string, vector, vector>>();
	}
	
	static FileDialogResult Import(out ref EditorWorldData data, string file)
	{
		COMFileData import_data = new COMFileData();
		JsonFileLoader<COMFileData>.JsonLoadFile(file, import_data);
		
		foreach (ref Param3<string, vector, vector> param: import_data.m_SceneObjects) {
			Print("ImportFromFile::COMFILE::Import " + param.param1);					
			data.EditorObjects.Insert(EditorObjectData.Create(param.param1, param.param2, param.param3));
		}
		
		return FileDialogResult.SUCCESS;
	}
	
	static FileDialogResult Export(ref EditorWorldData data, string file)
	{
		FileHandle handle = OpenFile(file, FileMode.WRITE);
		if (handle == 0)
			return FileDialogResult.UNKNOWN_ERROR;
		
		// SpawnObject("Land_Construction_House2", "6638.935547 7.190318 6076.024414", "146.000015 0.000000 0.000000")		
		foreach (EditorObjectData editor_object: data.EditorObjects) {
			string line = string.Format("SpawnObject(\"%1\", \"%2\", \"%3\");", editor_object.Type, editor_object.Position.ToString(false), editor_object.Orientation.ToString(false));
			FPrintln(handle, line);
		}
		
		return FileDialogResult.SUCCESS;
	}
}

class TerrainBuilderData
{
	
	static FileDialogResult Import(out ref EditorWorldData data, string file)
	{
		COMFileData import_data = new COMFileData();
		JsonFileLoader<COMFileData>.JsonLoadFile(file, import_data);
		

		
		return FileDialogResult.SUCCESS;
	}
	
	static FileDialogResult Export(ref EditorWorldData data, string file, ExportSettings export_settings)
	{
		FileHandle handle = OpenFile(file, FileMode.WRITE);
		if (handle == 0)
			return FileDialogResult.UNKNOWN_ERROR;
		
		// "construction_house2";206638.935547;6076.024414;146.000015;0.000000;0.000000;1.000000;
		// Name, X, Y, Yaw, Pitch, Roll, Scale, Relative Height
		
		foreach (EditorObjectData object_data: data.EditorObjects) {
			vector position = object_data.Position;
			
			if (export_settings.ExportHeightType == HeightType.RELATIVE)
				position[1] = position[1] - GetGame().SurfaceY(position[0], position[2]);
			
			position = position + export_settings.ExportOffset;
			
			string line = string.Format("\"%1\";%2;%3;%4;%5;%6;%7;%8;", object_data.GetModelName(), position[0], position[2], object_data.Orientation[0], object_data.Orientation[1], object_data.Orientation[2], object_data.Scale, position[1]);
			FPrintln(handle, line);

		}
		return FileDialogResult.SUCCESS;
	}
}





class EditorFileManager
{

	static FileDialogResult Save(ref EditorWorldData data, string file)
	{
		if (FileExist(file)) 
			if (!DeleteFile(file))
				return FileDialogResult.IN_USE;
		
		JsonFileLoader<ref EditorWorldData>.JsonSaveFile(file, data);
		return FileDialogResult.SUCCESS;
	}
	
	static FileDialogResult Open(out EditorWorldData data, string file)
	{
		if (!FileExist(file))
			return FileDialogResult.NOT_FOUND;
		
		JsonFileLoader<EditorWorldData>.JsonLoadFile(file, data);
		return FileDialogResult.SUCCESS;
	}
	
	static FileDialogResult Import(out ref EditorWorldData data, string file, ImportMode mode)
	{		
		if (!FileExist(file))
			return FileDialogResult.NOT_FOUND;
		
		
		switch (mode) {
			
			case ImportMode.COMFILE: {
				return COMFileData.Import(data, file);
			}
			
			case ImportMode.EXPANSION: {
				return ExpansionFileData.Import(data, file);
			}
			
			case ImportMode.TERRAINBUILDER: {
				return FileDialogResult.NOT_SUPPORTED;
			}
			
			case ImportMode.VPP: {
				return VPPFileDataImportData(data.EditorObjects, file);
			}
			
			default: {
				
				Print(string.Format("%1 not implemented!", typename.EnumToString(ImportMode, mode)));
				return FileDialogResult.NOT_SUPPORTED;
			}
		}

		
		return FileDialogResult.UNKNOWN_ERROR;
		
	}
	
	static FileDialogResult Export(ref EditorWorldData data, string file, ExportSettings export_settings)
	{
		Print("EditorFileManager::Export");
		
		if (FileExist(file)) 
			if (!DeleteFile(file))
				return FileDialogResult.IN_USE;
		
		switch (export_settings.ExportFileMode) {
			
			case ExportMode.TERRAINBUILDER: {
				return TerrainBuilderData.Export(data, file, export_settings);
			}
			
			case ExportMode.EXPANSION: {
				return ExpansionFileData.Export(data, file);
			}			
			
			case ExportMode.COMFILE: {
				return COMFileData.Export(data, file);
			}			
			
			case ExportMode.VPP: {
				return VPPFileDataExportData(data.EditorObjects, file, export_settings.ExportSetName);
			}
			
			default: {
				EditorPrint("ExportMode not supported! " + export_settings.ExportFileMode);
				return FileDialogResult.NOT_SUPPORTED;
			}
		}
		
		return FileDialogResult.UNKNOWN_ERROR;		
	}
}