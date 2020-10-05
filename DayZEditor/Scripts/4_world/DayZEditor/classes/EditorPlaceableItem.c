enum EditorPlaceableItemCategory {
	BUILDING = 0,
	VEHICLE = 1,
	ENTITY = 2,
	HUMAN = 3, 
	UNKNOWN = -1
};

static const ref TStringArray ITEM_BLACKLIST = 
{
	"access"	
};

class EditorPlaceableItem
{
	static const autoptr map<string, EditorPlaceableItemCategory> LOADED_TYPES = GetTypes();
	static const autoptr array<ref ModStructure> LOADED_MODS = ModLoader.GetMods();
	
	string Type;
	string Path;
	
	string Icon;
	string Model;
	
	ref TStringArray FullPath = {};
	
	ModStructure Mod;
	EditorPlaceableItemCategory Category;
	
	protected Object m_Object;
	
	private void EditorPlaceableItem(string path, string type)
	{
		EditorLog.Trace("EditorPlaceableItem");
		Path = path; Type = type;
		
		GetWorkbenchGame().ConfigGetText(string.Format("%1 %2 model", Path, Type), Model);
		GetWorkbenchGame().ConfigGetFullPath(string.Format("%1 %2", Path, Type), FullPath);
	}
	
	void Init()
	{
		EditorLog.Trace("EditorPlaceableItem::Init");
		
		
		// No .p3d was specified
		if (Model == string.Empty || Model.Length() <= 4) {
			delete this;
		}
		
		//m_Object = GetGame().CreateObjectEx(Type, vector.Zero, ECE_NONE);
		//m_Object.ClearFlags(EntityFlags.VISIBLE | EntityFlags.SOLID | EntityFlags.TOUCHTRIGGERS, false);
	}
	
	void ~EditorPlaceableItem()
	{
		EditorLog.Trace("~EditorPlaceableItem");
		delete FullPath;
		
		if (m_Object) {
			GetGame().ObjectDelete(m_Object);
		}
	}
	
	// CAN RETURN NULL
	static EditorPlaceableItem Create(string path, string type)
	{
		EditorPlaceableItem placeable_item = new EditorPlaceableItem(path, type);		
		placeable_item.Init();

		
		//Mod = LoadModData(Type, CfgPath);
		//Category = LoadItemCategory();
		
		/*if (placeable_item && placeable_item.Category == EditorPlaceableItemCategory.UNKNOWN) {
			EditorLog.Warning(string.Format("%1 has no category!", placeable_item.Type));
		}
		*/
		return placeable_item;
	}
	
	// If model volume is 0, return false
	private static bool IsValidObject(Object target)
	{
		vector size[2];
		target.ClippingInfo(size);
		return (Math.AbsFloat(size[0][0]) + Math.AbsFloat(size[1][0]) + Math.AbsFloat(size[0][1]) + Math.AbsFloat(size[1][1]) + Math.AbsFloat(size[0][2]) + Math.AbsFloat(size[1][2]) > 0);
	}
	
	private ModStructure LoadModData(string type, string cfg_path)
	{
		string model_path = GetGame().ConfigGetTextOut("CfgVehicles " + type + " model");
		if (model_path == "UNKNOWN_P3D_FILE") {
			EditorLog.Debug("Not loading mod data for %1", type);
			return null;
		}
	
		foreach (ModStructure mod: LOADED_MODS) {
			string dir;
			GetGame().ConfigGetText(string.Format("%1 dir", mod.GetModPath()), dir);
			Print(string.Format("%1 dir", mod.GetModPath()));
			dir.ToLower(); 
			model_path.ToLower();
			
			if (model_path.Contains(dir))
				return mod;
		}
		
		return null;
	}
	
	
	private EditorPlaceableItemCategory LoadItemCategory(string type, string cfg_path)
	{
		TStringArray path_array = {};
		GetGame().ConfigGetFullPath(string.Format("%1 %2", cfg_path, type), path_array);	
		/*
		foreach (string base: path_array) {
			base.ToLower();
			if (current_type.Find(base) + 1) {
				m_Category = i;	

			}
		}*/
	
		
		return -1;
	}
	
	
	static map<string, EditorPlaceableItemCategory> GetTypes()
	{
		map<string, EditorPlaceableItemCategory> types_map = new map<string, EditorPlaceableItemCategory>();
		
		types_map.Insert("house", 				EditorPlaceableItemCategory.BUILDING);
		types_map.Insert("transport", 			EditorPlaceableItemCategory.VEHICLE);
		types_map.Insert("inventory_base", 		EditorPlaceableItemCategory.ENTITY);
		types_map.Insert("edible_base", 		EditorPlaceableItemCategory.ENTITY);
		types_map.Insert("clothing_base", 		EditorPlaceableItemCategory.ENTITY);
		types_map.Insert("weapon_base", 		EditorPlaceableItemCategory.ENTITY);
		types_map.Insert("dz_lightai", 			EditorPlaceableItemCategory.HUMAN);
		types_map.Insert("survivorbase", 		EditorPlaceableItemCategory.HUMAN);
		
		return types_map;
	}
	
	// todo refactor
	static string GetIcon(ModStructure mod_info)
	{
		EditorLog.Trace("GetIcon");
		if (mod_info != null) {
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
		return "DayZEditor/gui/images/dayz_editor_icon_black.edds";
	}
}