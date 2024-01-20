class EditorObjectDataCategory: EditorPlaceableObjectData
{	
	static const string DEFAULT_ICON = "set:icons image:folder";
	
	static const ref EditorObjectDataCategory UNKNOWN = new EditorObjectDataCategory("Unknown", DEFAULT_ICON);
	
	static const ref EditorObjectDataCategory PLANTS = new EditorObjectDataCategory("Plants", "set:icons image:tree");		// \DZ\plants
	static const ref EditorObjectDataCategory ROCK = new EditorObjectDataCategory("Rocks");		// \DZ\rocks and \DZ\rocks_bliss
	static const ref EditorObjectDataCategory CLUTTER = new EditorObjectDataCategory("Clutter");	// 
	static const ref EditorObjectDataCategory STRUCTURE = new EditorObjectDataCategory("Structures", "set:icons image:house");	// HouseNoDestruct
	static const ref EditorObjectDataCategory WRECK = new EditorObjectDataCategory("Wrecks", "set:icons image:car_burst");		// \DZ\structures\Wrecks
	static const ref EditorObjectDataCategory AI = new EditorObjectDataCategory("AI", "set:icons image:person");			// DZ_LightAI
	static const ref EditorObjectDataCategory WATER = new EditorObjectDataCategory("Water", "set:icons image:water");		
	static const ref EditorObjectDataCategory VEHICLE = new EditorObjectDataCategory("Vehicles", "set:icons image:car");	// Car
	
	static const ref EditorObjectDataCategory STATIC = new EditorObjectDataCategory("Static Objects", "set:icons image:object_intersect");	// Static
	static const ref EditorObjectDataCategory DYNAMIC = new EditorObjectDataCategory("Dynamic Objects", "set:icons image:shirt");  	// Inventory_Base
	static const ref EditorObjectDataCategory SCRIPTED = new EditorObjectDataCategory("Scripted Objects", "set:icons image:code");
	
	static ref array<EditorObjectDataCategory> All = { UNKNOWN, PLANTS, ROCK, CLUTTER, STRUCTURE, WRECK, AI, WATER, VEHICLE, STATIC, DYNAMIC, SCRIPTED }; 
	
	protected string m_Name;
	protected string m_Icon;
	
	void EditorObjectDataCategory(string name, string icon = DEFAULT_ICON)
	{
		m_Name = name;
		m_Icon = icon;
	}
	
	override string GetName()
	{
		return m_Name;
	}
	
	override string GetIcon()
	{
		return m_Icon;
	}
}