


class Controller: Managed
{
	// Private members
	private ref PropertyHashMap m_ControllerPropertyHashMap;
	
	
	// Protected members
	protected Widget m_LayoutRoot;
	protected ref ViewBindingHashMap m_ViewBindingHashMap = new ViewBindingHashMap();
	protected ref PropertyHashMap m_PropertyHashMap;
	typename GetPropertyType(string property_name) {
		return m_PropertyHashMap.Get(property_name);
	}
	
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
			MVC.ErrorDialog("You cannot bind to data without creating your own controller class!");
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
			MVC.ErrorDialog("No DataBindings found! Is the controller in a parent Widget?");
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
	


	
	
	void NotifyPropertyChanged(string property_name)
	{
		EditorLog.Trace("Controller::NotifyPropertyChanged");
		ViewBinding view = m_ViewBindingHashMap.Get(property_name);
		
		if (!view) {
			MVC.ErrorDialog(string.Format("NotifyPropertyChanged: Property Not Found! %1", property_name));
			return;
		}
		
		view.OnPropertyChanged(this);
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
	

}











