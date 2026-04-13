enum EditorPlaceableItemCategory
{
	CONFIG = 0,
	STATIC = 1,
	SCRIPTED = 2
}

class EditorPlaceableItem : EditorNode
{
	int Scope;
	string Name;
	string Type; // Item Type
	string Path; // config path CfgVehicles, CfgWeapons etc...
	EditorPlaceableItemCategory Category;
	
	bool ScriptedTypeEx;
	bool ConsoleFriendly;

	private void EditorPlaceableItem()
	{
	}
	
	bool IsFavorite()
	{
		return GetEditor().GetSettings().FavoriteItems.Find(Type) != -1;
	}

	string GetName()
	{
		switch (Category)
		{
			case EditorPlaceableItemCategory.SCRIPTED:
			case EditorPlaceableItemCategory.CONFIG: return Type;
			case EditorPlaceableItemCategory.STATIC: return Path;
		}

		return string.Empty;
	}

	string GetSpawnType()
	{
		switch (Category)
		{
			case EditorPlaceableItemCategory.SCRIPTED:
			case EditorPlaceableItemCategory.CONFIG: return Type;
			case EditorPlaceableItemCategory.STATIC: return Path;
		}

		return string.Empty;
	}

	static EditorPlaceableItem Create(string p3d_file)
	{
		EditorPlaceableItem placeable_item = new EditorPlaceableItem();
		placeable_item.Scope = 2;
		placeable_item.Type = SystemPath.Format(p3d_file);
		placeable_item.Path = p3d_file;
		placeable_item.Name = File.GetName(p3d_file);
		placeable_item.Category = EditorPlaceableItemCategory.STATIC;
		return placeable_item;
	}

	static EditorPlaceableItem Create(string config_path, string config_type, int scope)
	{
		EditorPlaceableItem placeable_item = new EditorPlaceableItem();
		placeable_item.Scope = scope;
		placeable_item.Path = config_path;
		placeable_item.Type = config_type;
		placeable_item.Name = config_type;
		placeable_item.Category = EditorPlaceableItemCategory.CONFIG;

		return placeable_item;
	}

	static EditorPlaceableItem Create(typename scripted_type, bool console_friendly = true)
	{
		EditorPlaceableItem placeable_item = new EditorPlaceableItem();
		placeable_item.Scope = 2;
		placeable_item.Type = scripted_type.ToString();
		placeable_item.Name = scripted_type.ToString();
		placeable_item.Path = "Scripted/" + scripted_type.ToString();
		placeable_item.Category = EditorPlaceableItemCategory.SCRIPTED;
		placeable_item.ConsoleFriendly = console_friendly;
		placeable_item.ScriptedTypeEx = 1;
		return placeable_item;
	}

	// If model volume is 0, return false
	private static bool IsValidObject(Object target)
	{
		vector size[2];
		target.ClippingInfo(size);
		return (Math.AbsFloat(size[0][0]) + Math.AbsFloat(size[1][0]) + Math.AbsFloat(size[0][1]) + Math.AbsFloat(size[1][1]) + Math.AbsFloat(size[0][2]) + Math.AbsFloat(size[1][2]) > 0);
	}

	static string GetIcon(ModStructure mod_info)
	{
		//EditorLog.Trace("EditorPlaceableItem::GetIcon");
		if (mod_info)
		{
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

		//? Added a few more to the list
		/* 
		Give console Error: SCRIPT    (E): [WeaponStableState::ValidateMuzzleArray] :: 
		[ERROR] :: Muzzle array validation has failed. Please set up the correct muzzle states by overriding InitMuzzleArray.
		*/
		if (model == "Groza") return true;
		if (model == "PM73Rak") return true;
		if (model == "Trumpet") return true;
		//! abstract models 
		if (model == "Building") return true;
		if (model == "CarDoor") return true;
		if (model == "NonStrategic") return true;
		if (model == "Strategic") return true;
		if (model == "NonStrategic_Base") return true;
		if (model == "House") return true;
		if (model == "HouseNoDestruct") return true;
		if (model == "HouseHighCost") return true;
		if (model == "Ruins") return true;
		if (model == "EffectArea") return true;
		if (model == "SurrenderDummyItem") return true;
		if (model == "Head") return true;
		if (model == "Church") return true;
		if (model == "Land_Wreck_Car_TwoDoors") return true;
		if (model == "Land_Wreck_Car_ThreeDoors") return true;
		if (model == "EditorLootPoint") return true;
		if (model == "BoundingBoxBase") return true;
		if (model == "BrushBase") return true;
		if (model == "RotationWidget") return true;
		if (model == "M79DummyOptics") return true;
		if (model == "AugOptic") return true;
		if (model == "ThingEffect") return true;

		//! Everything is fine... I hope... :pain:
		return false;
	}
	
	string GetModelName()
	{		
		if (Category == EditorPlaceableItemCategory.SCRIPTED) {
			return Path;
		}
		
		if (Type.Contains(".p3d")) {
			return Type;
		}
		
		return g_Game.ConfigGetTextOut(string.Format("%1 %2 model", Path, Type));
	}
	
	override bool FilterType(string filter, bool favorites)
	{
		if (!filter && !favorites) {
			return true;
		}
		
		string search_string1 = Type;
		search_string1.ToLower();
		
		string search_string2 = Name;
		search_string2.ToLower();
			
		bool matches_filter = (search_string1.Contains(filter) || search_string2.Contains(filter) || !filter);
		if (favorites) {
			return (matches_filter && GetEditor().GetSettings().FavoriteItems.Find(Type) != -1);
		}
		
		return matches_filter;
	}
}