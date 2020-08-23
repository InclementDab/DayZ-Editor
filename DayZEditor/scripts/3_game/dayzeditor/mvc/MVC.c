
static ref MVC _MVC;

class MVC
{
	protected static ref TypeConverterHashMap m_TypeConverterHashMap;
	static TypeConverter GetTypeConversion(typename type) 
	{
		EditorLog.Trace("MVC::GetTypeConversion");
		if (!_MVC) {
			_MVC = new MVC();
		}
		// todo remove
		_MVC = new MVC();
		
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
	
	// Override THIS to add your own Custom Conversion Templates
	void RegisterConversionTemplates(out TypeConverterHashMap type_conversions)
	{
		EditorLog.Trace("MVC::RegisterConversionTemplates");
		type_conversions.Insert(bool, TypeConversionBool);
		type_conversions.Insert(float, TypeConversionFloat);
		type_conversions.Insert(string, TypeConversionString);
	}
	
			
	static void ErrorDialog(string message, string title = "Warning")
	{		
#ifdef COMPONENT_SYSTEM 
		
		// If Workbench is open
		Error(message);
		Workbench.Dialog(title, message);
#else
		EditorLog.Warning(message);
#endif
	}
	
	
	static void UnsupportedTypeError(typename type)
	{
		ErrorDialog(string.Format("Unsupported Type %1", type));
	}
	
	static void UnsupportedConversionError(typename from_type, typename to_type)
	{
		ErrorDialog(string.Format("Unsupported conversion from %1 to %2!", from_type, to_type));
	}

	
}

