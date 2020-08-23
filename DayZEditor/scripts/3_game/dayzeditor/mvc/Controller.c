




class DataBindingBase 
{
	private void DataBindingBase() {}
	
	ref PropertyInfo Property;
	ref ViewBinding View;	
	
	static DataBindingBase Create(ViewBinding view)
	{
		return GetDataType(view.GetRoot().Type());
	}
	
	private static DataBindingBase GetDataType(typename widget_type)
	{
		switch (widget_type) {
			
			case Widget:
			case SpacerBaseWidget:
				return new DataBinding<Widget>;
			
			case ButtonWidget:
			case CheckBoxWidget:
				return new DataBinding<bool>;
			
			case SliderWidget:
			case ProgressBarWidget:
			case SimpleProgressBarWidget:
				return new DataBinding<float>;
			
			case TextWidget:
			case ImageWidget:
			case EditBoxWidget:
			case HtmlWidget:
			case VideoWidget:
				return new DataBinding<string>;
			
			case RichTextWidget:
			case MultilineTextWidget:
			case MultilineEditBoxWidget:
			case XComboBoxWidget:
				return new DataBinding<TStringArray>;
			
			case ItemPreviewWidget:
				return new DataBinding<EntityAI>;
			
			case PlayerPreviewWidget:
				return new DataBinding<DayZPlayer>;
			
			default: {
				Error(string.Format("Unknown Type Specified %1", widget_type));
			}
		}
		
		return new DataBindingBase;
	}
}

class DataBinding<Class T>: DataBindingBase
{

}


// 0: Property Name
// 1: Proprety Type
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
	
	PropertyInfo GetPropertyInfo(string key)
	{
		return new PropertyInfo(Get(key), key);
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
		
		// Load all properties of the inheriting Controller
		m_PropertyHashMap = PropertyHashMap.FromType(Type());
		
		// Gets rid of properties that only exist in this class
		PropertyHashMap controller_map = PropertyHashMap.FromType(Controller);
		foreach (string name, typename type: controller_map) {
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
		
		m_DataBindingHashMap.DebugPrint();
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
	
	private void ErrorDialog(string message, string title = "Warning")
	{		
#ifdef COMPONENT_SYSTEM
		Workbench.Dialog(title, message);
#else
		EditorLog.Warning(message);
#endif
	}
}









