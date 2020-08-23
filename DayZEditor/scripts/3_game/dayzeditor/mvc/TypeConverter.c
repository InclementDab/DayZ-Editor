
class TypeConverterHashMap: ref map<typename, typename>
{
	void TypeConversionHashMap()
	{
		Insert(bool, TypeConversionBool);
		Insert(float, TypeConversionFloat);
		Insert(string, TypeConversionString);
	}
}

class TypeConverter
{	
	private static ref TypeConverterHashMap m_TypeConverterHashMap = new TypeConverterHashMap();
	static TypeConverter GetTypeConversion(typename type) { return m_TypeConverterHashMap.Get(type).Spawn(); }
	
	static bool RegisterTypeConversion(typename conversion_type, typename conversion_class)
	{		
		if (m_TypeConverterHashMap == null) {
			m_TypeConverterHashMap = new TypeConverterHashMap();
		}
		
		if (conversion_class.IsInherited(TypeConversionTemplate)) {
			return m_TypeConverterHashMap.Insert(conversion_type, conversion_class);
		} else {
			Error(string.Format("RegisterTypeConversion: %1 must inherit from type TypeConversionTemplate", conversion_class));
		}
		
		return false;
	}
	
	
	bool GetBool();
	float GetFloat();	
	string GetString();
	Widget GetWidget();
	
	void SetFromController(Class context, string name, int index);
}

class TypeConversionTemplate<Class T>: TypeConverter
{
	protected T value;
		
	T Get(Class context, string name, int index)
	{
		T _data;
		EnScript.GetClassVar(context, name, index, _data);
		return _data;
	}
	
	override void SetFromController(Class context, string name, int index)
	{
		EnScript.GetClassVar(context, name, index, value);
	}
}

class TypeConversionBool: TypeConversionTemplate<bool>
{
	override bool GetBool()
	{
		return value;
	}
}

class TypeConversionFloat: TypeConversionTemplate<float>
{
	override float GetFloat()
	{
		return value;
	}
}

class TypeConversionString: TypeConversionTemplate<string>
{	
	override bool GetBool()
	{
		return string.ToString(value, false, false, false) == "1";
	}
	
	override float GetFloat()
	{
		return string.ToString(value, false, false, false).ToFloat();
	}
	
	override string GetString()
	{
		return string.ToString(value, false, false, false);
	}
}



