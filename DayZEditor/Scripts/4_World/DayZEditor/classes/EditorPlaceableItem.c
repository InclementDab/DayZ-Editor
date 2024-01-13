enum EditorPlaceableItemCategory
{	
	UNKNOWN = -1,
	
	PLANTS,		// \DZ\plants
	ROCK,		// \DZ\rocks and \DZ\rocks_bliss
	CLUTTER,	// 
	STRUCTURES,	// HouseNoDestruct
	WRECK,		// \DZ\structures\Wrecks
	AI,			// DZ_LightAI
	WATER,		
	VEHICLE,	// Car
	
	STATIC,		// Static
	DYNAMIC,  	// Inventory_Base
	SCRIPTED,
}

class EditorPlaceableItem: Managed
{
	protected EditorPlaceableItemCategory m_Category;
		
	EditorObjectData CreateData(vector position, vector orientation, float scale, int flags)
	{
		return null;
	}
	
	Object CreateObject(vector position, vector orientation, float scale)	
	{
		return null;
	}
	
	EditorPlaceableItemCategory GetCategory()
	{
		return m_Category;
	}
	
	string GetName()
	{
		return string.Empty;
	}
	
	string GetModel()
	{
		return string.Empty;
	}
			
	static string GetIcon(ModStructure mod_info)
	{
		//EditorLog.Trace("EditorPlaceableItem::GetIcon");
		if (mod_info) {
			string logo = mod_info.GetModLogo();
			if (logo == string.Empty)
				logo = mod_info.GetModLogoSmall();
			if (logo == string.Empty)
				logo = mod_info.GetModLogoOver();
			if (logo == string.Empty)
				logo = mod_info.GetModActionURL();
			if (logo != string.Empty)
				return logo;	
		}
		// default
		return LIST_ITEM_DEFAULT_ICON;
	}
}