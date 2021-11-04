class EditorObjectSpawnerFile: EditorFileType
{
	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
		EditorLog.Trace("EditorObjectSpawnerFile::Export");		
		ObjectSpawnerData export_data = new ObjectSpawnerData();
		foreach (EditorObjectData object_data: data.EditorObjects) {
			export_data.Objects.Insert(new ObjectSpawnerEntry(object_data.Type, object_data.Position, object_data.Orientation));
		}
		
		JsonFileLoader<ObjectSpawnerData>.JsonSaveFile(file, export_data);
		
	}
	
	override EditorSaveData Import(string file, ImportSettings settings)
	{
		EditorLog.Trace("EditorObjectSpawnerFile::Import");		
		EditorSaveData save_data = new EditorSaveData();
		ObjectSpawnerData import_data = new ObjectSpawnerData();
		
		JsonFileLoader<ObjectSpawnerData>.JsonLoadFile(file, import_data);
		foreach (ObjectSpawnerEntry scene_object: import_data.Objects) {
			save_data.EditorObjects.Insert(EditorObjectData.Create(scene_object.name, scene_object.pos, scene_object.ypr, 1, EditorObjectFlags.ALL));
		}
		
		return save_data;
	}
	
	override string GetExtension() 
	{
		return ".json";
	}
}

class ObjectSpawnerEntry
{
	string name;
	vector pos;
	vector ypr;
	
	void ObjectSpawnerEntry(string _name, vector _pos, vector _ypr)
	{
		name = _name; pos = _pos; ypr = _ypr;
	}
}

class ObjectSpawnerData
{
	ref array<ref ObjectSpawnerEntry> Objects = {};
}