


class TypeConverterHashMap
{
	private ref map<typename, typename> value;
	
	void TypeConverterHashMap()
	{
		value = new map<typename, typename>();
	}
	
	typename Get(typename conversion_type)
	{
		return value.Get(conversion_type);
	}
	
	void Remove(typename conversion_type)
	{
		value.Remove(conversion_type);
	}
	
	void Set(typename conversion_type, typename conversion_class)
	{
		EditorLog.Trace("TypeConverterHashMap::Set");
		
		if (!conversion_class.IsInherited(TypeConversionTemplate)) {
			MVC.ErrorDialog(string.Format("RegisterTypeConversion: %1 must inherit from type TypeConversionTemplate", conversion_class));
			return;
		}
		
		value.Set(conversion_type, conversion_class);
	} 
	
	bool Insert(typename conversion_type, typename conversion_class)
	{
		EditorLog.Trace("TypeConverterHashMap::Insert");
		
		if (!conversion_class.IsInherited(TypeConversionTemplate)) {
			MVC.ErrorDialog(string.Format("RegisterTypeConversion: %1 must inherit from type TypeConversionTemplate", conversion_class));
			return false;
		}
		
		return value.Insert(conversion_type, conversion_class);
	}
}

class TypeConverter
{			
	bool GetBool();
	float GetFloat();	
	string GetString();
	Widget GetWidget();
	
	void SetToController(Class context, string name, int index);
	void GetFromController(Class context, string name, int index);
}

class TypeConversionTemplate<Class T>: TypeConverter
{
	protected T value;
	
	override void SetToController(Class context, string name, int index)
	{
		EnScript.SetClassVar(context, name, index, value);
	}
	
	override void GetFromController(Class context, string name, int index)
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



