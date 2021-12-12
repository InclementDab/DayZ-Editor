class EditorPlaceableItem
{	
	static autoptr map<string, EditorPlaceableItemCategory> LOADED_TYPES = GetTypes();
	static autoptr array<ref ModStructure> LOADED_MODS = ModLoader.GetMods();
	
	string Type;
	string Path;
	
	string Icon;
	string Model;
	string FullModel;
	
	ref TStringArray FullPath = {};
	
	ModStructure Mod;
	EditorPlaceableItemCategory Category;
	
	private void EditorPlaceableItem(string path, string type)
	{
		//EditorLog.Trace("EditorPlaceableItem");
		Path = path; Type = type;
		
		GetWorkbenchGame().ConfigGetText(string.Format("%1 %2 model", Path, Type), FullModel);
		GetWorkbenchGame().ConfigGetFullPath(string.Format("%1 %2", Path, Type), FullPath);
	}
	
	void Init(bool scripted = false)
	{
		//EditorLog.Trace("EditorPlaceableItem::Init");
		
		// No .p3d was specified
		if ((FullModel == string.Empty || FullModel.Length() <= 4) && !scripted) {
			delete this;
		}
		
		Mod = LoadModData(Type, Path);
		Category = LoadItemCategory();
		
		// format Model name
		Model = FullModel;
		array<string> model_split = {};
		Model.Split("\\", model_split);
		Model = model_split[model_split.Count() - 1];		
		//Icon = GetIcon(Mod);
	}
	
	void ~EditorPlaceableItem()
	{
		//EditorLog.Trace("~EditorPlaceableItem");
		delete FullPath;
	}
	
	// CAN RETURN NULL
	static EditorPlaceableItem Create(string path, string type)
	{
		if (IsForbiddenItem(type)) {
			return null;
		}
		
		EditorPlaceableItem placeable_item = new EditorPlaceableItem(path, type);		
		placeable_item.Init();
		
		if (placeable_item && placeable_item.Category == EditorPlaceableItemCategory.UNKNOWN) {
			// Todo: categories
			//EditorLog.Warning(string.Format("%1 has no category!", placeable_item.Type));
		}
		
		return placeable_item;
	}
	
	static EditorPlaceableItem Create(typename scripted_type)
	{		
		EditorPlaceableItem placeable_item = new EditorPlaceableItem("", scripted_type.ToString());		
		placeable_item.Init(true);
				
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
		if (!LOADED_MODS) {
			LOADED_MODS = ModLoader.GetMods();
		}
		
		foreach (ModStructure mod: LOADED_MODS) {
			string dir;
			GetGame().ConfigGetText(string.Format("%1 dir", mod.GetModPath()), dir);			
			if (Model.Contains(dir))
				return mod;
		}
		
		return null;
	}
	
	private EditorPlaceableItemCategory LoadItemCategory()
	{
		string path = GetGame().ConfigPathToString(FullPath);
		if (!LOADED_TYPES) {
			LOADED_TYPES = GetTypes();
		}
		
		foreach (string name, EditorPlaceableItemCategory category: LOADED_TYPES) {
			if (path.Contains(name)) {
				return category;
			}
		}

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
		if (GetGame().IsKindOf(model, "GP25Base")) return true;
		if (GetGame().IsKindOf(model, "M203Base")) return true;
	
		//! Everything is fine... I hope... :pain:
		return false;
	}
}