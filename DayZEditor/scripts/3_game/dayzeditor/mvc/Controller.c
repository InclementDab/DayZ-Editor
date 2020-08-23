


class Controller: Managed
{
	protected Widget m_LayoutRoot;
	protected ref DataBindingHashMap m_DataBindingHashMap = new DataBindingHashMap();
	protected ref PropertyHashMap m_PropertyHashMap = new PropertyHashMap();
	
	private ref PropertyHashMap m_ControllerPropertyHashMap = PropertyHashMap.FromType(Controller);
	
	void Controller()
	{
		EditorLog.Trace("Controller");
		
		// Load all properties of the inheriting Controller
		m_PropertyHashMap = PropertyHashMap.FromType(Type());
		
		// Gets rid of properties that only exist in this class
		foreach (string name, typename type: m_ControllerPropertyHashMap) {
			m_PropertyHashMap.Remove(name);
		}
		
		EditorLog.Info(string.Format("%1 Properties found!", m_PropertyHashMap.Count()));
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
		
		// Load all child Widgets and obtain their DataBinding class
		int binding_count = LoadDataBindings(m_LayoutRoot, m_DataBindingHashMap);
		if (binding_count == 0) {
			ErrorDialog("No DataBindings found! Is the controller in a parent Widget?");
			return;
		} else {
			EditorLog.Info(string.Format("%1 DataBindings found!", binding_count));
		}
		
		// debug
		m_DataBindingHashMap.DebugPrint();
		
		foreach (string name, DataBindingBase data: m_DataBindingHashMap) {
			PropertyInfo prop = m_PropertyHashMap.GetPropertyInfo(name);
			if (data.GetType() != prop.Type) {
				ErrorDialog(string.Format("Invalid data type in %1. Found %2, supports %3", name, prop.Type, data.GetType()));
				m_DataBindingHashMap.Remove(name);
			}
			
		}
		
	}
	
	
	void NotifyPropertyChanged(string property_name)
	{
		EditorLog.Trace("Controller::NotifyPropertyChanged");
		DataBindingBase data = m_DataBindingHashMap.Get(property_name);
		
		if (!data) {
			ErrorDialog(string.Format("NotifyPropertyChanged: Property Not Found! %1", property_name));
			return;
		}
		
	}
	


	
	
	private int LoadDataBindings(Widget w, out DataBindingHashMap binding_map)
	{
		ViewBinding view_binding;
		w.GetScript(view_binding);
		
		if (view_binding && view_binding.Type() == ViewBinding) {
			DataBindingBase data_binding = DataBinding.Create(view_binding);
			data_binding.Property = m_PropertyHashMap.GetPropertyInfo(view_binding.GetBindingName());
			binding_map.Insert(view_binding.GetBindingName(), data_binding);
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









