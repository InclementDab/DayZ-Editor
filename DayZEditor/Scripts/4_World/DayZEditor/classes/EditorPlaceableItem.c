enum EditorPlaceableItemCategory
{	
	UNKNOWN = -1,
	
	PLANTS,
	BUSH,
	ROCK,
	CLUTTER,
	STRUCTURES,
	WRECK,
	AI,
	WATER,
	VEHICLE,
	
	STATIC,
	DYNAMIC,
	SCRIPTED,
}

class EditorStaticPlaceableItem: EditorPlaceableItem
{
	protected ref CF_File m_Model;
	
	override EditorObjectData CreateData(vector position, vector orientation, float scale, int flags)
	{
		return EditorObjectData.Create(m_Type, position, orientation, scale, flags);
	}
}

class EditorConfigPlaceableItem: EditorPlaceableItem
{
	protected string m_Path, m_Type;
	
	string GetPath()
	{
		return m_Path;
	}
	
	string GetType()
	{
		return m_Type;
	}
	
	override EditorObjectData CreateData(vector position, vector orientation, float scale, int flags)
	{
		return EditorObjectData.Create(m_Type, position, orientation, scale, flags);
	}
}

class EditorPlaceableItem: Managed
{		
	protected EditorPlaceableItemCategory m_Category;
	
	private void EditorPlaceableItem();
		
	string GetType()
	{
		return string.Empty;
	}
		
	CF_File FindModel()
	{
		if (m_Model) {
			return m_Model;
		}
		
		string model;
		GetGame().ConfigGetText(string.Format("%1 %2 model", config_path, config_type), model);
		return new CF_File(model);
	}
	
	EditorObjectData CreateData(vector position, vector orientation, float scale, int flags)
	{
		return null;
	}
	
	static EditorPlaceableItem Create(notnull CF_File p3d)
	{
		EditorPlaceableItem placeable_item = new EditorPlaceableItem();	
		placeable_item.m_Type = p3d.GetFileName();
		placeable_item.m_Model = p3d;
		placeable_item.m_Category = EditorPlaceableItemCategory.STATIC;
		return placeable_item;
	}
	
	// CAN RETURN NULL
	static EditorPlaceableItem Create(string path, string type)
	{
		if (IsForbiddenItem(config_type)) {
			return null;
		}
		
		EditorPlaceableItem placeable_item = new EditorPlaceableItem();	
		placeable_item.m_Path = path;
		placeable_item.m_Type = type;
		
		//GetGame().ConfigIsExisting

				
		return placeable_item;
	}
	
	static EditorPlaceableItem Create(typename scripted_type)
	{		
		EditorPlaceableItem placeable_item = new EditorPlaceableItem();		
		placeable_item.m_Type = scripted_type.ToString();
		placeable_item.m_Category = EditorPlaceableItemCategory.SCRIPTED;
		return placeable_item;
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
	
	static bool IsForbiddenItem(string model)
	{
		//! In theory should be safe but just in case
		if (model.Contains("Fx")) return true;
		if (model == "ItemOptics") return true;

		//! Cursed items
		if (model == "AKM_TESTBED") return true;
		if (model == "Red9") return true;
		if (model == "QuickieBow") return true;
		if (model == "LargeTentBackPack") return true;
		if (model == "SurvivorMale_Base" || model == "SurvivorFemale_Base") return true;
		if (model == "Land_VASICore" || model == "FlagCarrierCore") return true;
		if (GetGame().IsKindOf(model, "GP25Base")) return true;
		if (GetGame().IsKindOf(model, "M203Base")) return true;
		if (model == "ItemOptics_Base") return true;
		
		//! Everything is fine... I hope... :pain:
		return false;
	}
}