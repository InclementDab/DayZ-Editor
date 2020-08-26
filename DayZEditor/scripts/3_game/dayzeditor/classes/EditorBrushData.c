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


modded class MVC
{
	override void RegisterConversionTemplates(out TypeConverterHashMap type_conversions)
	{
		super.RegisterConversionTemplates(type_conversions);
		type_conversions.Insert(EditorBrushData, TypeConversionBrush);
	}
}


class TypeConversionBrush: TypeConversionTemplate<ref EditorBrushData>
{
	
	override void SetString(string value) {
		m_Value.Name = value;
	}
	
	override string GetString() {
		return m_Value.Name;
	}
	
}

