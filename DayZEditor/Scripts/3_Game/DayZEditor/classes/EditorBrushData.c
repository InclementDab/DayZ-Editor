// This is the data that will be loaded from XML
class EditorBrushData
{
	string Name;
	float MinRadius, MaxRadius;
	typename BrushClassName;
	
	ref array<ref EditorBrushObject> PlaceableObjectTypes = {};
	
	bool InsertPlaceableObject(EditorBrushObject placeable_object)
	{
		//EditorLog.Trace("EditorBrushData::InsertPlaceableObject %1", placeable_object.Name);
		string model_name = GetGame().GetModelName(placeable_object.Name);
		if (model_name == "UNKNOWN_P3D_FILE") {
			EditorLog.Warning("%1 is not a valid Object Type!", placeable_object.Name);
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