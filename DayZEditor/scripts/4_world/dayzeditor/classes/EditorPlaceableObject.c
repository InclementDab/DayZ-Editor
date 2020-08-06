enum PlaceableObjectCategory
{
	BUILDING,
	VEHICLE,
	ENTITY,
	HUMAN, 
	UNKNOWN
}


static const ref array<string> BuildingTypes = {
	"HouseNoDestruct"
};

static const ref array<string> VehicleTypes = {
	"carscript"
};

static const ref array<string> EntityTypes = {
	"inventory_base"
};

static const ref array<string> HumanTypes = {
	"zombiemalebase",
	"zombiefemalebase"
};


class EditorPlaceableObject: Managed
{
	private string m_Type, m_Base;
	private Widget m_ListWidget;
	
	void EditorPlaceableObject(string name, string base)
	{
		m_Type = name; m_Base = base;

	}
	
	string GetType() { return m_Type; }
	string GetBase() { return m_Base; }
	
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
	
		if (BuildingTypes.Find(m_Base) + 1)
			return PlaceableObjectCategory.BUILDING;
		
		if (VehicleTypes.Find(m_Base) + 1)
			return PlaceableObjectCategory.VEHICLE;		
		
		if (EntityTypes.Find(m_Base) + 1)
			return PlaceableObjectCategory.ENTITY;		
		
		if (HumanTypes.Find(m_Base) + 1)
			return PlaceableObjectCategory.HUMAN;
		
		return PlaceableObjectCategory.UNKNOWN;
	}
}