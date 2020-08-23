


class TypeConverterHashMap: ref map<typename, typename>
{
	void TypeConversionHashMap()
	{
		MVC.RegisterTypeConversion(bool, TypeConversionBool);
		MVC.RegisterTypeConversion(float, TypeConversionFloat);
		MVC.RegisterTypeConversion(string, TypeConversionString);
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



