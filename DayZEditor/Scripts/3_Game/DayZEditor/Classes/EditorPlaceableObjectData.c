class EditorPlaceableObjectData: EditorSelectableBase
{
	protected Object m_Object;
	
	EditorObjectData CreateData(vector position, vector orientation, float scale, int flags = EFE_DEFAULT)
	{
		return null;
	}
	
	Object CreateObject(vector position, vector orientation, float scale, int flags = ECE_SETUP | ECE_LOCAL)	
	{
		return null;
	}
	
	EditorPlaceableObjectDataCategory GetCategory()
	{
		return EditorPlaceableObjectDataCategory.UNKNOWN;
	}
		
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
		
		if (selected) {
			m_Object = CreateObject(vector.Zero, vector.Zero, 1.0, ECE_LOCAL);
			GetEditor().AddInHand(m_Object, new EditorHandData());
		} else {
			GetEditor().RemoveFromHand(m_Object);
			GetGame().ObjectDelete(m_Object);
		}
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