enum EditorPlaceableItemCategory
{
	CONFIG = 0,
	STATIC = 1,
	SCRIPTED = 2
}

class EditorPlaceableItem : Managed
{
	string Type; // Item Type
	string Path; // config path
	EditorPlaceableItemCategory Category;

	private void EditorPlaceableItem()
	{
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
		placeable_item.Type = string.Format("p3d:%1", p3d_file);
		placeable_item.Path = p3d_file;
		placeable_item.Category = EditorPlaceableItemCategory.STATIC;
		return placeable_item;
	}

	static EditorPlaceableItem Create(string config_path, string config_type)
	{
		if (IsForbiddenItem(config_type))
		{
			return null;
		}

		EditorPlaceableItem placeable_item = new EditorPlaceableItem();
		placeable_item.Path = config_path;
		placeable_item.Type = config_type;
		placeable_item.Category = EditorPlaceableItemCategory.CONFIG;

		return placeable_item;
	}

	static EditorPlaceableItem Create(typename scripted_type)
	{
		EditorPlaceableItem placeable_item = new EditorPlaceableItem();
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
		//TODO add the abstract models 

		//! Everything is fine... I hope... :pain:
		return false;
	}
}