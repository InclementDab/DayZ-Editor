static vector GetP3dBoundingCenter(string p3d)
{
	Object object = GetGame().CreateStaticObjectUsingP3D(p3d, vector.Zero, vector.Zero, 1.0, true);
	if (!object) {
		return vector.Zero;
	}

	vector center = object.GetBoundingCenter();
	GetGame().ObjectDelete(object);
	return center;
}

class EditorObjectSpawnerFile : EditorFileType
{
	override void Export(EditorSaveData data, string file, ExportSettings settings, eDialogExtraSetting dialog_setting)
	{
		EditorLog.Trace("EditorObjectSpawnerFile::Export");
		ObjectSpawnerJson export_data = new ObjectSpawnerJson();
		export_data.Objects = { };
		foreach (EditorObjectData object_data: data.EditorObjects) {
			ITEM_SpawnerObject spawn_object = new ITEM_SpawnerObject();			
			string name = object_data.Type;
			// sheesh, DayZ is a lil crazy @ObjectSpawner.c:70
			if (name.Contains("dz")) {				
				name.Replace("dz", "DZ");
			}
			
			spawn_object.name = name;
			spawn_object.pos[0] = object_data.Position[0];
			spawn_object.pos[1] = object_data.Position[1];
			spawn_object.pos[2] = object_data.Position[2];
			spawn_object.ypr[0] = object_data.Orientation[0];
			spawn_object.ypr[1] = object_data.Orientation[1];
			spawn_object.ypr[2] = object_data.Orientation[2];
			spawn_object.scale = object_data.Scale;
			spawn_object.enableCEPersistency = false;

			if (spawn_object.name.Contains(".p3d")) {
				vector center = GetP3dBoundingCenter(spawn_object.name);
				spawn_object.pos[0] = object_data.Position[0] - center[0];
				spawn_object.pos[1] = object_data.Position[1] - center[1];
				spawn_object.pos[2] = object_data.Position[2] - center[2];
				spawn_object.ypr[0] = object_data.Orientation[0] * Math.DEG2RAD;
				spawn_object.ypr[1] = object_data.Orientation[1] * Math.DEG2RAD;
				spawn_object.ypr[2] = object_data.Orientation[2] * Math.DEG2RAD;
			}

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
			if (!scene_object.scale)
			{
				scene_object.scale = 1;
			}
			
			EditorObjectData dta = EditorObjectData.Create(scene_object.name, Vector(scene_object.pos[0], scene_object.pos[1], scene_object.pos[2]), Vector(scene_object.ypr[0], scene_object.ypr[1], scene_object.ypr[2]), scene_object.scale, EFE_DEFAULT);
			
			if (dta.Type.Contains(".p3d")) {
				vector center = GetP3dBoundingCenter(dta.Type);
				dta.Position = dta.Position + center;
				dta.Orientation = dta.Orientation * Math.RAD2DEG;
			}
			
			save_data.EditorObjects.Insert(dta);
		}

		return save_data;
	}

	override string GetExtension()
	{
		return ".json";
	}

	override void GetValidExtensions(notnull inout array<ref Param2<string, string>> valid_extensions)
	{
		super.GetValidExtensions(valid_extensions);
		valid_extensions.Insert(new Param2<string, string>("Text File", "*.txt"));
		valid_extensions.Insert(new Param2<string, string>("Object Spawner", "*.json"));
	}
}