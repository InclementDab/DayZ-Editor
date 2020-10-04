
enum PlaceableObjectCategory {
	BUILDING = 0,
	VEHICLE = 1,
	ENTITY = 2,
	HUMAN = 3, 
	UNKNOWN = -1
};


class EditorSettings
{
	
	static const ref TStringArray BuildingTypes = {
		"house"
	};

	static const ref TStringArray VehicleTypes = {
		"transport"
	};
	
	static const ref TStringArray EntityTypes = {
		"inventory_base",
		"edible_base",
		"clothing_base",
		"weapon_base"
	};
	
	static const ref TStringArray HumanTypes = {
		"dz_lightai",
		"survivorbase"
	};
	
	static const ref array<TStringArray> AllTypes = {
		BuildingTypes, 
		VehicleTypes,
		EntityTypes,
		HumanTypes
	};
	
	// shit thats gotta be changed
	float ViewDistance = 12000;
	float ObjectViewDistance = 5000;
	float MarkerViewDistance = 1500;
		
	// Autosave timer in seconds
	int AutoSaveTimer = 240;

	
	void EditorSettings() {
		EditorLog.Trace("EditorSettings");
	}
	
	static EditorSettings Load(string filename)
	{
		// todo load settings from ini or something
		return new EditorSettings();
	}
	
	
	static void Save(EditorSettings settings, string filename)
	{
		// todo save settings to ini or something
	}

	void Reload()
	{
		GetGame().GetWorld().SetViewDistance(ViewDistance);
		GetGame().GetWorld().SetObjectViewDistance(ObjectViewDistance);
	}

}

