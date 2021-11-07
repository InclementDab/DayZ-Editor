class EditorObjectSpawnerFile: EditorFileType
{
	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
		EditorLog.Trace("EditorObjectSpawnerFile::Export");		
		ObjectSpawnerJson export_data = new ObjectSpawnerJson();
		foreach (EditorObjectData object_data: data.EditorObjects) {
			ITEM_SpawnerObject spawn_object = new ITEM_SpawnerObject();
			spawn_object.name = object_data.Type;
			spawn_object.pos[0] = object_data.Position[0];
			spawn_object.pos[1] = object_data.Position[1];
			spawn_object.pos[2] = object_data.Position[2];
			spawn_object.ypr[0] = object_data.Orientation[0];
			spawn_object.ypr[1] = object_data.Orientation[1];
			spawn_object.ypr[2] = object_data.Orientation[2];
			export_data.Objects.Insert(spawn_object);
		}
		
		JsonFileLoader<ObjectSpawnerJson>.JsonSaveFile(file, export_data);
		
	}
	
	override EditorSaveData Import(string file, ImportSettings settings)
	{
		EditorLog.Trace("EditorObjectSpawnerFile::Import");		
		EditorSaveData save_data = new EditorSaveData();
		ObjectSpawnerJson import_data = new ObjectSpawnerJson();
		
		JsonFileLoader<ObjectSpawnerJson>.JsonLoadFile(file, import_data);
		foreach (ITEM_SpawnerObject scene_object: import_data.Objects) {
			save_data.EditorObjects.Insert(EditorObjectData.Create(scene_object.name, Vector(scene_object.pos[0], scene_object.pos[1], scene_object.pos[2]), Vector(scene_object.ypr[0], scene_object.ypr[1], scene_object.ypr[2]), 1, EditorObjectFlags.ALL));
		}
		
		return save_data;
	}
	
	override string GetExtension() 
	{
		return ".json";
	}
}