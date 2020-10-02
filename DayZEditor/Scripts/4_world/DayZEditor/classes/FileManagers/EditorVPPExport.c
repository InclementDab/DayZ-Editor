typedef FileSerializer Cerealizer;

static FileDialogResult ImportVPPData(out EditorObjectDataSet data, string filename)
{
	Cerealizer file = new Cerealizer();
	if (!FileExist(filename)) {
		return FileDialogResult.NOT_FOUND;
	}
	
	ref VPPToEditorBuildingSet bSet;
	if (file.Open(filename, FileMode.READ)) {
		file.Read(bSet);
		file.Close();
		
	} else return FileDialogResult.UNKNOWN_ERROR;
	
	if (!bSet) return FileDialogResult.UNKNOWN_ERROR;
	
	ref array<ref VPPToEditorSpawnedBuilding> spawned_buildings = new array<ref VPPToEditorSpawnedBuilding>();
	bSet.GetSpawnedBuildings(spawned_buildings);
	foreach (ref VPPToEditorSpawnedBuilding building: spawned_buildings) {
		string name = building.GetName();
		TStringArray name_split = new TStringArray();
		name.Split("-", name_split);
		data.InsertEditorData(EditorObjectData.Create(name_split.Get(0), building.GetPosition(), building.GetOrientation(), EditorObjectFlags.ALL));
	}
	
	return FileDialogResult.SUCCESS;
}

static FileDialogResult ExportVPPData(EditorObjectDataSet data, string filename, string set_name = "DayZEditor Export")
{
	Cerealizer file = new Cerealizer();
	
	ref VPPToEditorBuildingSet bSet = new VPPToEditorBuildingSet(set_name);
	
	foreach (EditorObjectData object_data: data) {
		bSet.AddBuilding(object_data.Type, object_data.GetPosition(), object_data.GetOrientation(), true);
		bSet.SetActive(true);
	}
	
	
	if (file.Open(filename, FileMode.APPEND)) {
		file.Write(bSet);
		file.Close();	
	}
	
	return FileDialogResult.SUCCESS;
}