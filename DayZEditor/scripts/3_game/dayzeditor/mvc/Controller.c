
typedef ref map<Widget, ref DataBinding> DataBindingHashMap;

class Controller: Managed
{
	protected Widget m_LayoutRoot;
	
	protected ref DataBindingHashMap m_DataBindingHashMap;
	
	void Controller()
	{
		EditorLog.Trace("Controller");
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("Controller::Init");
		
		m_LayoutRoot = w;
		
		

		Widget tree_parent = GetWidgetRoot(m_LayoutRoot);
		m_DataBindingHashMap = new DataBindingHashMap();
		
		LoadDataBindings(tree_parent, m_DataBindingHashMap);
		
#ifdef COMPONENT_SYSTEM
		if (m_DataBindingHashMap.Count() == 0) {
			Workbench.Dialog("Warning", "No DataBindings found! Make sure the Controller Widget is LAST");
		}
#else
		EditorLog.Debug(string.Format("%1 DataBindings found!", m_DataBindingHashMap.Count()));
#endif
	}
	
	
	private void LoadDataBindings(ref Widget w, ref DataBindingHashMap data_bindings)
	{
		//if (w == null) return;
		
		Print("Checking " + w.GetName());
		DataBinding data_binding;
		w.GetScript(data_binding);
		
		
		if (data_binding && data_binding.Type() == DataBinding) {
			Print("Script Found " + w.GetName());
			data_bindings.Insert(w, data_binding);
		}
		
		Widget children = w.GetChildren();
		if (children != null) {
			Print("Getting Child");
			LoadDataBindings(children, data_bindings);
		} 
		
		Widget sibling = w.GetSibling();
		if (sibling != null) {
			Print("Getting Sibling");
			LoadDataBindings(sibling, data_bindings);
		}
		
	

		
	}
	
	
}



