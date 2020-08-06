



class EditorSettings 
{
	static float VIEW_DISTANCE = 12000;
	static float OBJECT_VIEW_DISTANCE = 5000;
	
	static bool MAGNET_PLACEMENT = false;
	static bool MAINTAIN_HEIGHT = false;
	static bool SNAPPING_MODE = false;
	static bool SIM_CITY_MODE = false;

	
	private ref array<ref EditorPlaceableObject> m_AllPlaceableObjects;

	private PlaceableObjectCategory m_PlaceableObjectCategory;
	PlaceableObjectCategory GetPlaceableObjectCategory() { return m_PlaceableObjectCategory; }
	
	
	void EditorSettings()
	{
		Print("EditorSettings");
		// Load placeable objects		
		m_AllPlaceableObjects = new array<ref EditorPlaceableObject>(); 
		Print(string.Format("Loaded %1 Placeable Objects", EditorObjectManager.GetPlaceableObjects(m_AllPlaceableObjects)));
	}
	
	
	void GetPlaceableObjectsByCategory(out ref array<ref EditorPlaceableObject> target, PlaceableObjectCategory category)
	{
		Print("EditorSettings::GetPlaceableObjectsByCategory");
		foreach (ref EditorPlaceableObject placeable_object: m_AllPlaceableObjects)
			if (category == placeable_object.GetCategory() || (category == PlaceableObjectCategory.ENTITY && placeable_object.GetCategory() == PlaceableObjectCategory.UNKNOWN))
				target.Insert(placeable_object);
	}
	
	
	static void Load()
	{
		GetGame().GetWorld().SetViewDistance(VIEW_DISTANCE);
		GetGame().GetWorld().SetObjectViewDistance(OBJECT_VIEW_DISTANCE);
		
		// todo load em from ini file
	}
	
	
	static void Save()
	{
		// todo save settings to ini or something
	}
	
	void SetPlaceableObjectCategory(PlaceableObjectCategory category) 
	{
		m_PlaceableObjectCategory = category;
		EditorEvents.PlaceableCategoryChangedInvoke(this, category);
	}
	
	
	
}