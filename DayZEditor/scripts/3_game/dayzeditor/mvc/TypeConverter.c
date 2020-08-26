


class TypeConverterHashMap
{
	private ref map<typename, typename> value;
	
	void TypeConverterHashMap()
	{
		EditorLog.Trace("TypeConverterHashMap");
		value = new map<typename, typename>();
	}
	
	void ~TypeConverterHashMap() { EditorLog.Trace("~TypeConverterHashMap"); }
	
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
	void TypeConverter() { EditorLog.Trace("TypeConverter"); }
	void ~TypeConverter() { EditorLog.Trace("~TypeConverter"); }
	typename GetType();
	
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
	override typename GetType() {
		return T;
	}
	
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
		return m_Value.ToString();
	}
	
	override void SetBool(bool value)
	{
		m_Value = value;
	}
	
	override void SetString(string value)
	{
		value.ToLower();
		m_Value = (value == "true");
	}
}

class TypeConversionFloat: TypeConversionTemplate<float>
{
	override bool GetBool()
	{
		return (m_Value == 1);
	}
	
	override void SetBool(bool value)
	{
		m_Value = value;
	}
	
	override float GetFloat()
	{
		return m_Value;
	}
	
	override void SetFloat(float value)
	{
		m_Value = value;
	}
	
	override string GetString()
	{
		return string.ToString(m_Value, false, false, false);
	}
	
	override void SetString(string value)
	{
		m_Value = value.ToFloat();
	}
	
}

class TypeConversionString: TypeConversionTemplate<string>
{	
	override bool GetBool()
	{
		string value = string.ToString(m_Value, false, false, false);
		value.ToLower();
		return (value == "true");
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
		m_Value = value.ToString();
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

class TypeConversionWidget: TypeConversionTemplate<Widget>
{
	override void SetWidget(Widget value)
	{
		m_Value = value;
	}
	
	override Widget GetWidget()
	{
		return m_Value;
	}
}

class TypeConversionCollection: TypeConversionTemplate<ObservableCollection<string>>
{
	
	override void SetString(string value) {
		TStringArray split_array = new TStringArray();
		value.Split("\n", split_array);
		foreach (string line: split_array)
			m_Value.Insert(line);
		
	}
	
	override string GetString()	{
		string result;
		for (int i = 0; i < m_Value.Count(); i++) 
			if (i != m_Value.Count() - 1)
				result += m_Value.Get(i) + "\n";
			else 
				result += m_Value.Get(i);
		
		return result;
	}
}
