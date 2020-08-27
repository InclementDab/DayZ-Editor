
static ref MVC _MVC;

class MVC
{
	protected static ref TypeConverterHashMap m_TypeConverterHashMap;
	static TypeConverter GetTypeConversion(typename type) 
	{
		if (!_MVC) {
			_MVC = new MVC();
		}
		// todo remove
		//_MVC = new MVC();
		
		return m_TypeConverterHashMap.Get(type).Spawn(); 
	}
	
	void MVC()
	{
		EditorLog.Trace("MVC");
		if (!m_TypeConverterHashMap) {
			m_TypeConverterHashMap = new TypeConverterHashMap();
			RegisterConversionTemplates(m_TypeConverterHashMap);
		}
	}
	
	void ~MVC() { EditorLog.Trace("~MVC"); }
	
	// Override THIS to add your own Custom Conversion Templates
	void RegisterConversionTemplates(out TypeConverterHashMap type_conversions)
	{
		EditorLog.Trace("MVC::RegisterConversionTemplates");
		type_conversions.Insert(bool, TypeConversionBool);
		type_conversions.Insert(int, TypeConversionInt);
		type_conversions.Insert(float, TypeConversionFloat);
		type_conversions.Insert(string, TypeConversionString);
		type_conversions.Insert(vector, TypeConversionVector);
		type_conversions.Insert(Widget, TypeConversionWidget);
	}
	
	
	static void PropertyNotFoundError(string property_name)
	{
		EditorLog.Info("Property '%1' not found", property_name);
	}
	
	static void UnsupportedTypeError(typename type)
	{
		EditorLog.Error("Unsupported Type: %1", type.ToString());
	}
	
	static void UnsupportedConversionError(typename from_type, typename to_type)
	{
		EditorLog.Error("Unsupported conversion from %1 to %2!", from_type.ToString(), to_type.ToString());
	}
	
	static void TypeConversionError(typename property_type)
	{
		if (!property_type) {
			EditorLog.Warning("Data Binding not found!");
		} else {
			EditorLog.Warning(string.Format("Could not find TypeConversion for Type %1\nUse MVC.RegisterConversionTemplates to register custom types", property_type.ToString()));
		}
	}
	

	
}

