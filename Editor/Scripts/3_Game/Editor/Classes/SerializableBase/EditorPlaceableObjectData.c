class EditorPlaceableObjectData: SerializableBase
{	
	EditorObjectData CreateData(vector position, vector orientation, float scale, int flags = EFE_DEFAULT)
	{
		return null;
	}
	
	Object CreateObject(vector position, vector orientation, float scale, int flags = ECE_SETUP | ECE_LOCAL)	
	{
		return null;
	}
	
	EditorObjectDataCategory GetCategory()
	{
		return EditorObjectDataCategory.UNKNOWN;
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
		//EditorLog.Trace("EditorPlaceableObjectData::GetIcon");
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