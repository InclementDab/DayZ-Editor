typedef ref map<string, ref EditorBrushData> EditorBrushDataSet;

// This is the data that will be loaded from XML
class EditorBrushData
{
	string Name;
	float MinRadius, MaxRadius;
	ref TStringArray PlaceableObjects = new TStringArray();
	
	typename BrushClassName;
	
	bool InsertPlaceableObject(string object_name, float object_frequency)
	{
		string model_name = GetGame().GetModelName(object_name);
		if (model_name == "UNKNOWN_P3D_FILE") {
			Print(string.Format("%1 is not a valid Object Type!", object_name));
			return false;
		}

		for (int i = 0; i < object_frequency * 100; i++)
			PlaceableObjects.Insert(object_name);
		
		return true;
	}
}






