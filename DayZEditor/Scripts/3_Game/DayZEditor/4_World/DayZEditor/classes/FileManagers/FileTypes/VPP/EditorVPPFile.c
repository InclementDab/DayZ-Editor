
class EditorVPPFile: EditorFileType
{
	override EditorSaveData Import(string file, ImportSettings settings)
	{
		EditorLog.Trace("EditorVPPFile::Import");
		
		EditorSaveData save_data = new EditorSaveData();
		FileSerializer file_serializer = new FileSerializer();
	
		VPPToEditorBuildingSet building_set;
		if (!file_serializer.Open(file, FileMode.READ)) {
			return save_data;
		}
		
		file_serializer.Read(building_set);
		file_serializer.Close();
		
		if (!building_set) {
			return save_data;
		}
		
		array<ref VPPToEditorSpawnedBuilding> spawned_buildings = new array<ref VPPToEditorSpawnedBuilding>();
		building_set.GetSpawnedBuildings(spawned_buildings);
		foreach (VPPToEditorSpawnedBuilding building: spawned_buildings) {
			string name = building.GetName();
			TStringArray name_split = new TStringArray();
			name.Split("-", name_split);
			save_data.EditorObjects.Insert(EditorObjectData.Create(name_split.Get(0), building.GetPosition(), building.GetOrientation(), 1, EFE_DEFAULT));
		}
		
		return save_data;
	}
	
	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
		EditorLog.Trace("EditorVPPFile::Export");
		
		FileSerializer file_serializer = new FileSerializer();
	
		VPPToEditorBuildingSet building_set = new VPPToEditorBuildingSet(settings.ExportSetName);
		
		foreach (EditorObjectData object_data: data.EditorObjects) {
			building_set.AddBuilding(object_data.Type, object_data.Position, object_data.Orientation, true);
			building_set.SetActive(true);
		}
		
		if (!file_serializer.Open(file, FileMode.APPEND)) {
			EditorLog.Error("VPPFile::Export: Error Opening File Serializer");
			return;
		}
		
		file_serializer.Write(building_set);
		file_serializer.Close();
	}
	
	override string GetExtension() 
	{
		return ".vpp";
	}
}