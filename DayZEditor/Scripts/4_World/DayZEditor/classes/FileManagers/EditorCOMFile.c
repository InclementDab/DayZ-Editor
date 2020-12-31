
class EditorCOMFile: EditorFileType
{
	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
		EditorLog.Trace("EditorCOMFile::Export");
		
		COMImportData export_data = new COMImportData();
		export_data.name = "DayZ Editor Export";
		foreach (EditorObjectData object_data: data.EditorObjects) {
			export_data.m_SceneObjects.Insert(new Param3<string, vector, vector>(object_data.Type, object_data.Position, object_data.Orientation));
		}
		
		JsonFileLoader<COMImportData>.JsonSaveFile(file, export_data);
	}
	
	override EditorSaveData Import(string file, ImportSettings settings)
	{
		EditorLog.Trace("EditorCOMFile::Import");
		
		EditorSaveData save_data = new EditorSaveData();
		COMImportData import_data = new COMImportData();
		
		JsonFileLoader<COMImportData>.JsonLoadFile(file, import_data);
		
		
		foreach (Param3<string, vector, vector> scene_object: import_data.m_SceneObjects) {
			save_data.EditorObjects.Insert(EditorObjectData.Create(scene_object.param1, scene_object.param2, scene_object.param3, 1, EditorObjectFlags.ALL));
		}
		
		return save_data;
	}
	
	override string GetExtension() {
		return ".json";
	}
}

class COMImportData
{
	string name;
	ref array<ref Param3<string, vector, vector>> m_SceneObjects;
	
	void COMImportData()
	{
		m_SceneObjects = new array<ref Param3<string, vector, vector>>();
	}
}
