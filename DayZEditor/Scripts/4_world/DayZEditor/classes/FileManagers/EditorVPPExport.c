typedef FileSerializer Cerealizer;

static EditorFileResult ImportVPPData(out EditorObjectDataMap data, string filename)
{
	Cerealizer file = new Cerealizer();
	
	ref VPPToEditorBuildingSet bSet;
	if (file.Open(filename, FileMode.READ)) {
		file.Read(bSet);
		file.Close();
		
	} else return EditorFileResult.UNKNOWN_ERROR;
	
	if (!bSet) return EditorFileResult.UNKNOWN_ERROR;
	
	ref array<ref VPPToEditorSpawnedBuilding> spawned_buildings = new array<ref VPPToEditorSpawnedBuilding>();
	bSet.GetSpawnedBuildings(spawned_buildings);
	foreach (ref VPPToEditorSpawnedBuilding building: spawned_buildings) {
		string name = building.GetName();
		TStringArray name_split = new TStringArray();
		name.Split("-", name_split);
		EditorObjectData dta = EditorObjectData.Create(name_split.Get(0), building.GetPosition(), building.GetOrientation(), EditorObjectFlags.ALL);
		data.Insert(dta.GetID(), dta);
	}
	
	return EditorFileResult.SUCCESS;
}

static EditorFileResult ExportVPPData(EditorObjectDataMap data, string filename, string set_name = "DayZEditor Export")
{
	Cerealizer file = new Cerealizer();
	
	ref VPPToEditorBuildingSet bSet = new VPPToEditorBuildingSet(set_name);
	
	foreach (EditorObjectData object_data: data) {
		bSet.AddBuilding(object_data.Type, object_data.Position, object_data.Orientation, true);
		bSet.SetActive(true);
	}
	
	
	if (file.Open(filename, FileMode.APPEND)) {
		file.Write(bSet);
		file.Close();	
	}
	
	return EditorFileResult.SUCCESS;
}