
class EditorVPPFile: EditorFileType
{
	override EditorSaveData Import(string file, ImportSettings settings)
	{
		EditorSaveData save_data = new EditorSaveData();
		FileSerializer file_serializer = new FileSerializer();
	
		VPPToEditorBuildingSet building_set;
		if (!file_serializer.Open(file, FileMode.READ)) {
			return save_data;
		}
		
		file_serializer.Read(building_set);
		file_serializer.Close();
		
		if (!building_set) return save_data;
		
		ref array<ref VPPToEditorSpawnedBuilding> spawned_buildings = new array<ref VPPToEditorSpawnedBuilding>();
		building_set.GetSpawnedBuildings(spawned_buildings);
		foreach (ref VPPToEditorSpawnedBuilding building: spawned_buildings) {
			string name = building.GetName();
			TStringArray name_split = new TStringArray();
			name.Split("-", name_split);
			save_data.EditorObjects.Insert(EditorObjectData.Create(name_split.Get(0), building.GetPosition(), building.GetOrientation(), EditorObjectFlags.ALL));
		}
		
		return save_data;
	}
	
	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
		FileSerializer file_serializer = new FileSerializer();
	
		VPPToEditorBuildingSet building_set = new VPPToEditorBuildingSet(settings.ExportSetName);
		
		foreach (EditorObjectData object_data: data.EditorObjects) {
			building_set.AddBuilding(object_data.Type, object_data.Position, object_data.Orientation, true);
			building_set.SetActive(true);
		}
		
		Print(building_set);
		
		if (!file_serializer.Open(file, FileMode.APPEND)) {
			EditorLog.Error("VPPFile::Export: Error Opening File Serializer");
			return;
		}
		
		file_serializer.Write(building_set);
		file_serializer.Close();
	}
	
	string GetExtension() {
		return ".vpp";
	}
}