typedef ref map<string, ref EditorBrushData> EditorBrushDataSet;


typedef ref array<ref EditorBrushObject> EditorBrushObjectArray;

class EditorBrushObject
{
	string Name;
	float Frequency;
	float ZOffset;
	
	void EditorBrushObject(string name, float frequency, float zoffset) {
		Name = name; Frequency = frequency; ZOffset = zoffset;
	}
}

// This is the data that will be loaded from XML
class EditorBrushData
{
	string Name;
	float MinRadius, MaxRadius;
	
	ref EditorBrushObjectArray PlaceableObjectTypes;

	typename BrushClassName;
	
	bool InsertPlaceableObject(EditorBrushObject placeable_object)
	{
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
		EditorBrushObjectArray PlaceableObjects = new EditorBrushObjectArray();
		
		// This is rly slow
		foreach (EditorBrushObject placeable_object: PlaceableObjectTypes)
			for (int j = 0; j < placeable_object.Frequency * 100; j++)
				PlaceableObjects.Insert(placeable_object);
		
		
		return PlaceableObjects.GetRandomElement();
	}
}






