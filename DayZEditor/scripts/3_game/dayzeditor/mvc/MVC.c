


class MVC
{
	protected static ref TypeConverterHashMap m_TypeConverterHashMap;
	static TypeConverter GetTypeConversion(typename type) { return m_TypeConverterHashMap.Get(type).Spawn(); }
	
	void MVC()
	{
		EditorLog.Trace("MVC");
		if (!m_TypeConverterHashMap) {
			m_TypeConverterHashMap = new TypeConverterHashMap();
		}
	}
	
	static bool RegisterTypeConversion(typename conversion_type, typename conversion_class)
	{
		EditorLog.Trace("MVC::RegisterTypeConversion");
		if (conversion_class.IsInherited(TypeConversionTemplate)) {
			return m_TypeConverterHashMap.Insert(conversion_type, conversion_class);
		} else {
			Error(string.Format("RegisterTypeConversion: %1 must inherit from type TypeConversionTemplate", conversion_class));
		}
		
		return false;
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

