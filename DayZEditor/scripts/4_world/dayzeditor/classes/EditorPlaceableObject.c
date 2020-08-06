enum PlaceableObjectCategory
{
	BUILDING,
	VEHICLE,
	ENTITY,
	HUMAN, 
	UNKNOWN
}


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

class EditorPlaceableObject: Managed
{
	private string m_Type, m_Path, m_Base;
	private PlaceableObjectCategory m_Category;
	private Widget m_ListWidget;
	

	void EditorPlaceableObject(string type, string path)
	{
		m_Type = type; m_Path = path;
		
		TStringArray path_array = new TStringArray();
		GetGame().ConfigGetFullPath(m_Path + " " + m_Type, path_array);

		int i = 0;
		foreach (array<string> current_type: AllTypes) {
			foreach (string base: path_array) {
				base.ToLower();
				if (current_type.Find(base) + 1) {
					m_Base = base;
					m_Category = i;
					return;
				}
			}
			i++;
		}
		
		Print(string.Format("%1 has no category!", m_Type));
		m_Category = PlaceableObjectCategory.UNKNOWN;
		
	
	}
	
	string GetType() { return m_Type; }
	//string GetBase() { return m_Base; }
	
	ref EditorListItem GetListItem(Widget parent)
	{
		ref EditorListItem list_item;
		m_ListWidget = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorListItem.layout", parent);
		m_ListWidget.GetScript(list_item);
		list_item.SetObject(this);
		return list_item;
	}
	
	PlaceableObjectCategory GetCategory()
	{
		return m_Category;
	}
}