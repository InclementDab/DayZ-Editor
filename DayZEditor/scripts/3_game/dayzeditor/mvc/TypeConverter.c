


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
	
	void SetBool(bool value);
	void SetFloat(float value);
	void SetString(string value);
	void SetWidget(Widget value);
	
	void SetToController(Class context, string name, int index);
	void GetFromController(Class context, string name, int index);
}

class TypeConversionTemplate<Class T>: TypeConverter
{
	protected T m_Value;
	
	override void SetToController(Class context, string name, int index)
	{
		EnScript.SetClassVar(context, name, index, m_Value);
	}
	
	override void GetFromController(Class context, string name, int index)
	{
		EnScript.GetClassVar(context, name, index, m_Value);
	}
}

class TypeConversionBool: TypeConversionTemplate<bool>
{
	override bool GetBool()
	{
		return m_Value;
	}
	
	override string GetString()
	{
		return string.ToString(m_Value, false, false, false);
	}
	
	override void SetBool(bool value)
	{
		m_Value = value;
	}
	
	override void SetString(string value)
	{
		m_Value = (value == "1");
	}
}

class TypeConversionFloat: TypeConversionTemplate<float>
{
	override float GetFloat()
	{
		return m_Value;
	}
	
	override void SetFloat(float value)
	{
		m_Value = value;
	}
}

class TypeConversionString: TypeConversionTemplate<string>
{	
	override bool GetBool()
	{
		return string.ToString(m_Value, false, false, false) == "1";
	}
	
	override float GetFloat()
	{
		return string.ToString(m_Value, false, false, false).ToFloat();
	}
	
	override string GetString()
	{
		return string.ToString(m_Value, false, false, false);
	}
	
	override void SetBool(bool value)
	{
		if (value)
			m_Value = "1";
		else 
			m_Value = "0";
	}
	
	override void SetFloat(float value)
	{	
		m_Value = value.ToString();
	}
	
	override void SetString(string value)
	{
		m_Value = value;
	}
	

}



