class EditorPlaceableObjectDataCategory: EditorCategory
{	
	static const ref EditorPlaceableObjectDataCategory UNKNOWN = new EditorPlaceableObjectDataCategory("Unknown");
	
	static const ref EditorPlaceableObjectDataCategory PLANTS = new EditorPlaceableObjectDataCategory("Plants");		// \DZ\plants
	static const ref EditorPlaceableObjectDataCategory ROCK = new EditorPlaceableObjectDataCategory("Rocks");		// \DZ\rocks and \DZ\rocks_bliss
	static const ref EditorPlaceableObjectDataCategory CLUTTER = new EditorPlaceableObjectDataCategory("Clutter");	// 
	static const ref EditorPlaceableObjectDataCategory STRUCTURE = new EditorPlaceableObjectDataCategory("Structures");	// HouseNoDestruct
	static const ref EditorPlaceableObjectDataCategory WRECK = new EditorPlaceableObjectDataCategory("Wrecks");		// \DZ\structures\Wrecks
	static const ref EditorPlaceableObjectDataCategory AI = new EditorPlaceableObjectDataCategory("AI");			// DZ_LightAI
	static const ref EditorPlaceableObjectDataCategory WATER = new EditorPlaceableObjectDataCategory("Water");		
	static const ref EditorPlaceableObjectDataCategory VEHICLE = new EditorPlaceableObjectDataCategory("Vehicles");	// Car
	
	static const ref EditorPlaceableObjectDataCategory STATIC = new EditorPlaceableObjectDataCategory("Static Objects");	// Static
	static const ref EditorPlaceableObjectDataCategory DYNAMIC = new EditorPlaceableObjectDataCategory("Dynamic Objects");  	// Inventory_Base
	static const ref EditorPlaceableObjectDataCategory SCRIPTED = new EditorPlaceableObjectDataCategory("Scripted Objects");
	
	static ref array<EditorPlaceableObjectDataCategory> All = { UNKNOWN, PLANTS, ROCK, CLUTTER, STRUCTURE, WRECK, AI, WATER, VEHICLE, STATIC, DYNAMIC, SCRIPTED }; 
}