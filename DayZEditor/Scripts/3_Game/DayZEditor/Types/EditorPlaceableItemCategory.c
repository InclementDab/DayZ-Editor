class EditorPlaceableItemCategory: EditorCategory
{	
	static const ref EditorPlaceableItemCategory UNKNOWN = new EditorPlaceableItemCategory("Unknown");
	
	static const ref EditorPlaceableItemCategory PLANTS = new EditorPlaceableItemCategory("Plants");		// \DZ\plants
	static const ref EditorPlaceableItemCategory ROCK = new EditorPlaceableItemCategory("Rocks");		// \DZ\rocks and \DZ\rocks_bliss
	static const ref EditorPlaceableItemCategory CLUTTER = new EditorPlaceableItemCategory("Clutter");	// 
	static const ref EditorPlaceableItemCategory STRUCTURE = new EditorPlaceableItemCategory("Structures");	// HouseNoDestruct
	static const ref EditorPlaceableItemCategory WRECK = new EditorPlaceableItemCategory("Wrecks");		// \DZ\structures\Wrecks
	static const ref EditorPlaceableItemCategory AI = new EditorPlaceableItemCategory("AI");			// DZ_LightAI
	static const ref EditorPlaceableItemCategory WATER = new EditorPlaceableItemCategory("Water");		
	static const ref EditorPlaceableItemCategory VEHICLE = new EditorPlaceableItemCategory("Vehicles");	// Car
	
	static const ref EditorPlaceableItemCategory STATIC = new EditorPlaceableItemCategory("Static Objects");	// Static
	static const ref EditorPlaceableItemCategory DYNAMIC = new EditorPlaceableItemCategory("Dynamic Objects");  	// Inventory_Base
	static const ref EditorPlaceableItemCategory SCRIPTED = new EditorPlaceableItemCategory("Scripted Objects");
}