


typedef ref array<ref EditorPlaceableObject> EditorPlaceableObjectSet;
typedef ref map<int, ref EditorPlaceableObjectSet> EditorPlaceableObjectsData;


class EditorSettings 
{
	// shit thats gotta be changed
	static float VIEW_DISTANCE = 12000;
	static float OBJECT_VIEW_DISTANCE = 5000;
	
	static bool MAGNET_PLACEMENT = false;
	static bool MAINTAIN_HEIGHT = false;
	static bool SNAPPING_MODE = false;
	static bool SIM_CITY_MODE = false;

	
	// private members
	
	private ref EditorPlaceableObjectsData m_PlaceableObjectsData;
	EditorPlaceableObjectsData GetPlaceableObjectsData() { return m_PlaceableObjectsData; }
	
	private PlaceableObjectCategory m_PlaceableObjectCategory;
	PlaceableObjectCategory GetPlaceableObjectCategory() { return m_PlaceableObjectCategory; }
	
	
	void EditorSettings()
	{
		Print("EditorSettings");
		// Load placeable objects		
		m_PlaceableObjectsData = new EditorPlaceableObjectsData();
		
		for (int i = 0; i <= PlaceableObjectCategory.UNKNOWN; i++) {
			m_PlaceableObjectsData.Insert(i, new EditorPlaceableObjectSet());
		}
		
		Print(string.Format("Loaded %1 Placeable Objects", EditorObjectManager.GetPlaceableObjects(m_PlaceableObjectsData)));
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