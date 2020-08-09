


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

class EditorPlaceableObject: Managed
{
	// Private members
	private string m_Type, m_Path, m_Base;
	
	private ModInfo m_ModInfo;
	
	private PlaceableObjectCategory m_Category;
	private Widget m_ListWidget;
	private ref EditorListItem m_ListItem;

	// Getters
	string GetType() { return m_Type; }	
	ModInfo GetModInfo() { return m_ModInfo; }
	
	void EditorPlaceableObject(string type, string path)
	{
		m_Type = type; m_Path = path;
		m_ModInfo = Editor.GetModFromObject(m_Type);
		
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
	
	
	
	ref EditorListItem SetListItem(Widget parent)
	{
		m_ListWidget = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorListItem.layout", parent);
		m_ListWidget.GetScript(m_ListItem);
		m_ListItem.SetObject(this);
		string logo = m_ModInfo.GetLogo();
		if (logo == string.Empty)
			logo = m_ModInfo.GetLogoSmall();
		if (logo == string.Empty)
			logo = m_ModInfo.GetLogoOver();
		if (logo == string.Empty)
			logo = m_ModInfo.GetPicture();
		if (logo != string.Empty)
			m_ListItem.SetIcon(logo);	
		else m_ListItem.SetIcon("DayZEditor/gui/images/dayz_editor_icon_black.edds");
		
		return m_ListItem;
	}
	
	ref EditorListItem GetListItem()
	{
		return m_ListItem;
	}
	
	PlaceableObjectCategory GetCategory()
	{
		return m_Category;
	}
}