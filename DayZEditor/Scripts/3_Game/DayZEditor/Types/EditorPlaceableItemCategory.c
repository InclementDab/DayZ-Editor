typedef int EditorPlaceableItemCategory;
class EditorPlaceableItemCategory: int
{	
	static const int UNKNOWN = 0;
	
	static const int PLANTS = 1;		// \DZ\plants
	static const int ROCK = 2;		// \DZ\rocks and \DZ\rocks_bliss
	static const int CLUTTER = 3;	// 
	static const int STRUCTURE = 4;	// HouseNoDestruct
	static const int WRECK = 5;		// \DZ\structures\Wrecks
	static const int AI = 6;			// DZ_LightAI
	static const int WATER = 7;		
	static const int VEHICLE = 8;	// Car
	
	static const int STATIC = 9;	// Static
	static const int DYNAMIC = 10;  	// Inventory_Base
	static const int SCRIPTED = 11;
	
	string GetDisplayName()
	{
		switch (value) {
			case UNKNOWN: return "Unknown";
			case PLANTS: return "Plants";
			case ROCK: return "Rocks";
			case CLUTTER: return "Clutter";
			case STRUCTURE: return "Structures";
			case WRECK: return "Wrecks";
			case AI: return "AI";
			case WATER: return "Water";
			case VEHICLE: return "Vehicles";
			case STATIC: return "Static Objects";
			case DYNAMIC: return "Dynamic Object";
			case SCRIPTED: return "Scripted Objects";
		}
		
		return string.Empty;
	}
}