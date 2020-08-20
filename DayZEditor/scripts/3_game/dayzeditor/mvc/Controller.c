
typedef ref map<Widget, ref ViewBinding> DataBindingHashMap;

class PropertyHashMap: map<string, typename>
{
		
	static PropertyHashMap FromType(typename type)
	{
		PropertyHashMap hash_map = new PropertyHashMap();
		for (int i = 0; i < type.GetVariableCount(); i++) {
			hash_map.Insert(type.GetVariableName(i), type.GetVariableType(i));
		}
		
		return hash_map;
	}
	
}

class Controller: Managed
{
	protected Widget m_LayoutRoot;
	protected ref DataBindingHashMap m_DataBindingHashMap = new DataBindingHashMap();
	protected ref PropertyHashMap m_PropertyHashMap = new PropertyHashMap();
	
	
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
		
		// Load all child Widgets and obtain their DataBinding class
		int binding_count = LoadDataBindings(m_LayoutRoot, m_DataBindingHashMap);
		if (binding_count == 0) {
			ErrorDialog("No DataBindings found! Is the controller in the Root Widget?");
			return;
		} else {
			EditorLog.Info(string.Format("%1 DataBindings found!", binding_count));
		}
		

		// Load all properties of the inheriting Controller
		m_PropertyHashMap = PropertyHashMap.FromType(Type());		
		EditorLog.Info(string.Format("%1 Properties found!", m_PropertyHashMap.Count()));

	}
	
	
	private int LoadDataBindings(Widget w, out DataBindingHashMap binding_map)
	{
		ViewBinding data_binding;
		w.GetScript(data_binding);
		
		if (data_binding && data_binding.Type() == ViewBinding) {
			binding_map.Insert(w, data_binding);
		}
		
		if (w.GetChildren() != null) {
			LoadDataBindings(w.GetChildren(), binding_map);
		} 
		
		if (w.GetSibling() != null) {
			LoadDataBindings(w.GetSibling(), binding_map);
		}		
		
		return binding_map.Count();
	}
	
	private void ErrorDialog(string message, string title = "Warning")
	{
#ifdef COMPONENT_SYSTEM
		Workbench.Dialog(title, message);
#endif
	}
	

}



