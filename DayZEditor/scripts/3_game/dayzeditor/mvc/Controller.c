


class Controller: Managed
{
	// Private members
	private ref PropertyHashMap m_ControllerPropertyHashMap;
	
	
	// Protected members
	protected Widget m_LayoutRoot;
	protected ref ViewBindingHashMap m_ViewBindingHashMap = new ViewBindingHashMap();
	protected ref PropertyHashMap m_PropertyHashMap;
	
	
	void Controller()
	{
		EditorLog.Trace("Controller"); 
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("Controller::Init");
		m_LayoutRoot = w;
				
		// User must inherit from controller, not use it in ScriptClass
		if (Type() == Controller) {
			ErrorDialog("You cannot bind to data without creating your own controller class!");
			return;
		}
		
		// Load all properties of the inheriting Controller
		m_PropertyHashMap = PropertyHashMap.FromType(Type());
		
		// Gets rid of properties that only exist in this class
		m_ControllerPropertyHashMap = PropertyHashMap.FromType(Controller);
		// Commented due to crashes for whatever reason smile
		//foreach (string name, typename type: m_ControllerPropertyHashMap) {
		//	m_PropertyHashMap.Remove(name);
		//}
		
		EditorLog.Info(string.Format("%1 Properties found!", m_PropertyHashMap.Count()));
		
		// Load all child Widgets and obtain their DataBinding class
		int binding_count = LoadDataBindings(m_LayoutRoot, m_ViewBindingHashMap);
		if (binding_count == 0) {
			ErrorDialog("No DataBindings found! Is the controller in a parent Widget?");
			return;
		} else {
			EditorLog.Info(string.Format("%1 DataBindings found!", binding_count));
		}
		
		// debug
		m_ViewBindingHashMap.DebugPrint();
		/*
		foreach (string data_name, DataBindingBase data: m_DataBindingHashMap) {
			PropertyInfo prop = m_PropertyHashMap.GetPropertyInfo(data_name);
			if (!data.CanConvertFrom(prop.Type)) {
				ErrorDialog(string.Format("Invalid data type in %1. Found %2, supports %3", data_name, prop.Type, data.GetType()));
				m_DataBindingHashMap.Remove(data_name);
			}
		}*/
				
	}
	

	void UnsupportedConversionError(typename from_type, typename to_type)
	{
		ErrorDialog(string.Format("Unsupported conversion from %1 to %2!", from_type, to_type));
	}
	
	
	void NotifyPropertyChanged(string property_name)
	{
		EditorLog.Trace("Controller::NotifyPropertyChanged");
		ViewBinding view = m_ViewBindingHashMap.Get(property_name);
		
		if (!view) {
			ErrorDialog(string.Format("NotifyPropertyChanged: Property Not Found! %1", property_name));
			return;
		}
		
		
		EditorLog.Debug("NotifyPropertyChanged::SetData");
		
		typename property_type = m_PropertyHashMap.Get(view.GetBindingName());
		typename widgetdata_type = view.GetWidgetDataType();
		
		
		// If the property of the Controller is the native widget data type

			
		if (widgetdata_type != property_type) {
			EditorLog.Debug(string.Format("Attempting conversion from %1 to %2", property_type, widgetdata_type));
		}
		
		switch (property_type) {		
					
			case bool: {
				WidgetDataConverter<bool> _WidgetDataConverterBool(this, view.GetBindingName(), view.GetBindingIndex());
				switch (widgetdata_type) {
					
					case bool: {
						view.Set(_WidgetDataConverterBool.Get());
						break;
					}
					
					case float: {
						view.Set(_WidgetDataConverterBool.ToFloat());
						break;
					}
					
					case string: {
						view.Set(_WidgetDataConverterBool._ToString());
						break;
					}
														
					default: {
						UnsupportedConversionError(property_type, widgetdata_type);
					}
				}
				break;
			}
			
			case float: {
				WidgetDataConverter<float> _WidgetDataConverterFloat(this, view.GetBindingName(), view.GetBindingIndex());
				switch (widgetdata_type) {
										
					case bool: {
						view.Set(_WidgetDataConverterFloat.ToBool());
						break;
					}
					
					case float: {
						view.Set(_WidgetDataConverterFloat.Get());
						break;
					}
					
					case string: {
						view.Set(_WidgetDataConverterFloat._ToString());
						break;
					}
					
					default: {
						UnsupportedConversionError(property_type, widgetdata_type);
					}
				}
				
			}
			
			case string: {
				
				WidgetDataConverter<string> _WidgetDataConverterString(this, view.GetBindingName(), view.GetBindingIndex());
				switch (widgetdata_type) {
										
					case bool: {
						view.Set(_WidgetDataConverterString.ToBool());
						break;
					}
					
					case float: {
						view.Set(_WidgetDataConverterString.ToFloat());
						break;
					}
					
					case string: {
						view.Set(_WidgetDataConverterString.Get());
						break;
					}
					
					default: {
						UnsupportedConversionError(property_type, widgetdata_type);
					}
				}
				
				break;
			}		
		}
	}
		
	private int LoadDataBindings(Widget w, out ViewBindingHashMap binding_map)
	{
		ViewBinding view_binding;
		w.GetScript(view_binding);
		
		if (view_binding && view_binding.Type() == ViewBinding) {
			binding_map.Insert(view_binding.GetBindingName(), view_binding);
		}
		
		if (w.GetChildren() != null) {
			LoadDataBindings(w.GetChildren(), binding_map);
		} 
		
		if (w.GetSibling() != null) {
			LoadDataBindings(w.GetSibling(), binding_map);
		}		
		
		return binding_map.Count();
	}
	
	static void ErrorDialog(string message, string title = "Warning")
	{		
#ifdef COMPONENT_SYSTEM
		Error(message);
		Workbench.Dialog(title, message);
#else
		EditorLog.Warning(message);
#endif
	}
}











