

static ref map<typename, ref Controller> ControllerHashMap;

typedef ref map<string, ref DataBinding> DataBindingHashMap;
typedef ref map<string, typename> PropertyHashMap;

// Inherit this class, then put that class into ScriptClass for your View Model Widget
class Controller: Managed
{
	// Widget Data
	protected Widget m_LayoutRoot;
	
	// Hashed Variable Data
	protected ref PropertyHashMap m_PropertyHashMap;
	
	// View List
	protected ref DataBindingHashMap m_DataBindings;
	DataBindingHashMap GetDataBindings() { return m_DataBindings; }
	
	private ControllerData m_Data;
	
	
	void Controller()
	{ 
		EditorLog.Trace("Controller"); 
		m_Data = data;
		
		if (ControllerHashMap == null)
			ControllerHashMap = new map<typename, ref Controller>();
		
		ControllerHashMap.Insert(Type(), this);
		
		m_PropertyHashMap = new PropertyHashMap();
		m_DataBindings = new DataBindingHashMap();
	}
	
	void ~Controller() { EditorLog.Trace("~Controller"); }
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("Controller::OnWidgetScriptInit");
		
#ifdef COMPONENT_SYSTEM
		if (Type() == Controller) {
			Workbench.Dialog("Error", "Cannot use ScriptClass to type Controller. You must inherit it!");
			return;
		}
#endif
		
		m_LayoutRoot = w;
		
		
		typename vtype = Type();
		int vcnt = vtype.GetVariableCount();
		for (int i = 0; i < vcnt; i++)
			m_PropertyHashMap.Insert(vtype.GetVariableName(i), vtype.GetVariableType(i));		
		EditorLog.Info(string.Format("Controller::Init: %1 properties found!", i));
		
		EnumerateDataBindings(m_DataBindings, m_LayoutRoot);
		EditorLog.Info(string.Format("Controller::Init: %1 data bindings found!", m_DataBindings.Count()));
		
		
		NotifyOnPropertyChanged(OnPropertyChanged);
		NotifyOnCollectionChanged(OnCollectionChanged);
	}
	
	
	void EnumerateDataBindings(out DataBindingHashMap data_bindings, Widget w)	
	{
		if (w == null) return;
		
		Class script;
		w.GetScript(script);
		
		if (script != null && script.IsInherited(DataBinding)) {
			DataBinding binding = DataBinding.Cast(script);
			//data_bindings.Insert(binding.DataBindingName, binding);
		}
		
		EnumerateDataBindings(data_bindings, w.GetChildren());
		EnumerateDataBindings(data_bindings, w.GetSibling());
	}
	
	
	void OnPropertyChanged(string property_name)
	{
		//EditorLog.Trace(string.Format("Controller::NotifyPropertyChanged: %1", property_name));
		//m_EditorViewList.Get(property_name).OnPropertyChanged();
		//m_BindingList.Get(property_name).OnPropertyChanged();
	}
	
	
	void OnCollectionChanged(string property_name, CollectionChangedEventArgs args)
	{
		EditorLog.Trace(string.Format("Controller::OnCollectionChanged: %1 Action: %2", property_name, args.param2));
		//m_EditorViewList.Get(property_name).OnCollectionChanged(args);	
	}
	
	
	
	static typename GetTypeFromWidgetSource(WidgetSource source)
	{
		_GetTypeFromWidgetSource(source);
		return _GetTypeResult;
	}
	
	private static typename _GetTypeResult;
	private static void _GetTypeFromWidgetSource(WidgetSource source)
	{
		if (!source) return;
			
		int index = source.VarIndex("scriptclass");
		
		if (source.IsVariableSet(index)) {
			string script;
			source.Get(index, script);
			typename type = script.ToType();
			if (type.IsInherited(Controller)) {
				Print("Controller Found");
				_GetTypeResult = type;
				return;
			}		
		}
		
		_GetTypeFromWidgetSource(source.GetChildren());
		_GetTypeFromWidgetSource(source.GetSibling());
	}
	
	

	
	static WidgetSource GetWidgetSource()
	{
#ifdef COMPONENT_SYSTEM
		ResourceBrowser m_Module = Workbench.GetModule("ResourceManager");
		return m_Module.GetContainer();
#endif
	}
	
	
	bool OnClick(Widget w, int x, int y, bool button) { return true; }
	
	void DebugPrint()
	{
		EditorLog.Debug("Controller::DebugPrint: " + m_LayoutRoot.GetName());
		foreach (string name, DataBinding view: m_DataBindings)
			view.DebugPrint();
				
	}
}

// property_name = name of variable being changed
ref ScriptInvoker PropertyChanged;
void NotifyPropertyChanged(string property_name)
{
	PropertyChanged.Invoke(property_name);
}


void NotifyOnPropertyChanged(func action)
{	
	if (PropertyChanged == null)
		PropertyChanged = new ScriptInvoker();
	
	PropertyChanged.Insert(action);

}


// Called only when Collection is changed
// Use NotifyOnPropertyChanged to 'subscribe' to the Invoke
ref ScriptInvoker CollectionChanged;
void NotifyOnCollectionChanged(func action)
{
	if (CollectionChanged == null)
		CollectionChanged = new ScriptInvoker();
	
	CollectionChanged.Insert(action);
}






static ref IEditorViewOptionsCallback m_IEditorViewOptionsCallback = new IEditorViewOptionsCallback();

class IEditorViewOptionsCallback 
{
	
	void IEditorViewOptionsCallback()
	{
		m_IEditorViewOptionsCallback = this;
		EditorViewOptionsCallback.Instance = this;
		EditorViewOptionsCallback.SearchAction = "ResourceSearch";
	}
	
	DataBindingHashMap ResourceSearch()
	{
		Controller m_Controller;
		ResourceBrowser m_Module = Workbench.GetModule("ResourceManager");
		WidgetSource widget = m_Module.GetContainer();
		
		if (ControllerHashMap != null) {
			m_Controller = ControllerHashMap.Get(Controller.GetTypeFromWidgetSource(widget));
			
			if (m_Controller != null) {
				Print("Controller found! Enumerating views...");
				// success
				return m_Controller.GetDataBindings();
			}
		}
		
		// Fail condition
		Workbench.Dialog("oof!", "Controller not found!");
		return null;
	}	
	
}



