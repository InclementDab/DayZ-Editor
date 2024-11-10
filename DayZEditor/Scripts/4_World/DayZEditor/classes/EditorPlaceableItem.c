enum EditorPlaceableItemCategory
{
	CONFIG = 0,
	STATIC = 1,
	SCRIPTED = 2
}

class EditorPlaceableItem : Managed
{
	int Scope;
	string Type; // Item Type
	string Path; // config path
	EditorPlaceableItemCategory Category;

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
		placeable_item.Type = string.Format("p3d:%1", p3d_file);
		placeable_item.Path = p3d_file;
		placeable_item.Category = EditorPlaceableItemCategory.STATIC;
		return placeable_item;
	}

	static EditorPlaceableItem Create(string config_path, string config_type, int scope)
	{
		if (IsForbiddenItem(config_type))
		{
			return null;
		}

		EditorPlaceableItem placeable_item = new EditorPlaceableItem();
		placeable_item.Scope = scope;
		placeable_item.Path = config_path;
		placeable_item.Type = config_type;
		placeable_item.Category = EditorPlaceableItemCategory.CONFIG;

		return placeable_item;
	}

	static EditorPlaceableItem Create(typename scripted_type)
	{
		EditorPlaceableItem placeable_item = new EditorPlaceableItem();
		placeable_item.Scope = 2;
		placeable_item.Type = scripted_type.ToString();
		placeable_item.Category = EditorPlaceableItemCategory.SCRIPTED;
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
		model.ToLower();
		model.TrimInPlace();
		//! In theory should be safe but just in case
		if (model == "itemoptics") return true;
		if (model == "access") return true;
		if (model == "transport") return true;
		if (model == "all") return true;

		//! Cursed items
		if (model == "akm_testbed") return true;
		if (model == "red9") return true;
		if (model == "quickiebow") return true;
		if (model == "largetentbackpack") return true;
		if (model == "survivormale_base" || model == "survivorfemale_base") return true;
		if (model == "land_vasicore" || model == "flagcarriercore") return true;
		if (GetGame().IsKindOf(model, "gp25base")) return true;
		if (GetGame().IsKindOf(model, "m203base")) return true;
		if (model == "itemoptics_base") return true;

		//? Added a few more to the list
		/* 
		Give console Error: SCRIPT    (E): [WeaponStableState::ValidateMuzzleArray] :: 
		[ERROR] :: Muzzle array validation has failed. Please set up the correct muzzle states by overriding InitMuzzleArray.
		*/
		if (model == "groza") return true;
		if (model == "pm73rak") return true;
		if (model == "trumpet") return true;
		//TODO add the abstract models 

		//! Everything is fine... I hope... :pain:
		return false;
	}
}