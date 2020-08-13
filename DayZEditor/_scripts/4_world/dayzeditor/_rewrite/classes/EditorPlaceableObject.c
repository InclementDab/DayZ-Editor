

enum PlaceableObjectCategory {
	BUILDING,
	VEHICLE,
	ENTITY,
	HUMAN, 
	UNKNOWN
};


static const ref array<string> BuildingTypes = {
	"house"
};

static const ref array<string> VehicleTypes = {
	"transport"
};

static const ref array<string> EntityTypes = {
	"inventory_base",
	"edible_base",
	"clothing_base",
	"weapon_base"
};

static const ref array<string> HumanTypes = {
	"dz_lightai",
	"survivorbase"
};

static const ref array<array<string>> AllTypes = {
	BuildingTypes, 
	VehicleTypes,
	EntityTypes,
	HumanTypes
};

class EditorPlaceableObjectData
{
	// Private members
	string Type, Path, Base;
	
	private ModStructure m_ModStructure;
	private PlaceableObjectCategory m_Category;
	
	void EditorPlaceableObjectData(string type, string path)
	{
		Type = type; Path = path;
		
		
		// probably really slow
		m_ModStructure = Editor.GetModFromObject(Type);
		
		TStringArray path_array = new TStringArray();
		GetGame().ConfigGetFullPath(Path + " " + Type, path_array);

		int i = 0;
		foreach (array<string> current_type: AllTypes) {
			foreach (string base: path_array) {
				base.ToLower();
				if (current_type.Find(base) + 1) {
					Base = base;
					m_Category = i;	
					return;
				}
			}
			i++;
		}
		
		Print(string.Format("%1 has no category!", Type));
		m_Category = PlaceableObjectCategory.UNKNOWN;		
	}
	
	
	PlaceableObjectCategory GetCategory() { return m_Category; }
	ModStructure GetModStructure() { return m_ModStructure; }
}