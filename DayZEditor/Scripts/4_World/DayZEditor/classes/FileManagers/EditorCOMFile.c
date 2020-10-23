
class EditorCOMFile: EditorFileType
{
	override EditorSaveData Import(string file, ImportSettings settings)
	{
		EditorSaveData save_data = new EditorSaveData();
		COMImportData import_data = new COMImportData();
		
		JsonFileLoader<COMImportData>.JsonLoadFile(file, import_data);
		
		
		foreach (Param3<string, vector, vector> scene_object: import_data.m_SceneObjects) {
			Print(scene_object);
			save_data.EditorObjects.Insert(EditorObjectData.Create(scene_object.param1, scene_object.param2, scene_object.param3, EditorObjectFlags.ALL));
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
