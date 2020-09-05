typedef ref map<string, ref EditorBrushData> EditorBrushDataSet;


typedef ref array<ref EditorBrushObject> EditorBrushObjectArray;
class EditorBrushObject
{
	string Name;
	float ZOffset;
	
	void EditorBrushObject(string name, float zoffset) {
		Name = name; ZOffset = zoffset;
	}
}

// This is the data that will be loaded from XML
class EditorBrushData
{
	string Name;
	float MinRadius, MaxRadius;
	ref EditorBrushObjectArray PlaceableObjects = new EditorBrushObjectArray();
	
	typename BrushClassName;
	
	bool InsertPlaceableObject(EditorBrushObject placeable_object, float object_frequency)
	{
		string model_name = GetGame().GetModelName(placeable_object.Name);
		if (model_name == "UNKNOWN_P3D_FILE") {
			EditorLog.Warning("%1 is not a valid Object Type!", placeable_object.Name);
			return false;
		}

		for (int i = 0; i < object_frequency * 100; i++)
			PlaceableObjects.Insert(placeable_object);
		
		return true;
	}
}






