// This is the data that will be loaded from XML
class EditorBrushData: Managed
{
	string Type;
	float MinRadius, MaxRadius;
	typename BrushClassName;
	
	ref array<ref EditorBrushObject> PlaceableObjectTypes = {};
	
	bool InsertPlaceableObject(EditorBrushObject placeable_object)
	{
		// directly check for p3d in files
		if (!FileExist(placeable_object.Type)) {
			EditorLog.Warning("BrushObject %1 is not a valid p3d", placeable_object.Type);
			return false;
		}
		
		PlaceableObjectTypes.Insert(placeable_object);
		return true;
	}
	
	EditorBrushObject GetRandomObject()
	{
		EditorBrushObject brush_object = PlaceableObjectTypes.GetRandomElement();
		
		if (PlaceableObjectTypes.Count() == 0) return null;
		while (Math.RandomFloat01() > brush_object.Frequency)
			brush_object = PlaceableObjectTypes.GetRandomElement();
	
		return brush_object;
	}
}