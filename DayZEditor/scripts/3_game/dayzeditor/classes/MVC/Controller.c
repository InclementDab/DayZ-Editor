
typedef ref array<ref EditorView> TEditorViewSet;
static ref map<typename, Controller> ControllerHashMap;

typedef ref map<string, ref EditorViewBase> EditorViewHashMap;

	

// Inherit this class, then put that class into ScriptClass for your View Model Widget
class Controller: Managed
{
	// Widget Data
	protected Widget m_LayoutRoot;
	protected Widget m_ControllerWidget;
	
	// Hashed Variable Data
	protected ref map<string, typename> m_ModelHashMap;
	protected ref map<string, ref ViewBinding> m_BindingList;
	
	// View List
	protected ref EditorViewHashMap m_EditorViewList;
	EditorViewHashMap GetEditorViewList() { return m_EditorViewList; }
	
	void Controller() { EditorLog.Trace("Controller"); }
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
		
		m_ControllerWidget = w;
		m_LayoutRoot = GetWidgetRoot(m_ControllerWidget);
		
		m_ModelHashMap = new map<string, typename>();
		m_BindingList = new map<string, ref ViewBinding>();
		m_EditorViewList = new EditorViewHashMap();
		
		typename vtype = Type();
		int vcnt = vtype.GetVariableCount();
		for (int i = 0; i < vcnt; i++)
			m_ModelHashMap.Insert(vtype.GetVariableName(i), vtype.GetVariableType(i));		
		
		if (ControllerHashMap == null) {
			ControllerHashMap = new map<typename, Controller>();
		}
		
		ControllerHashMap.Insert(Type(), this);
				
		NotifyOnPropertyChanged(OnPropertyChanged);
		NotifyOnCollectionChanged(OnCollectionChanged);
	}
	
	void InsertView(ref EditorView view)
	{
		string variable_name = view.GetVariableName();
		EditorLog.Trace("Controller::InsertView: " + variable_name);
		
		Print(m_EditorViewList.Insert(variable_name, view));
	}
	
	void InsertBinding(ViewBinding binding)
	{
		string variable_name = binding.GetVariableName();
		EditorLog.Trace("Controller::InsertBinding: " + variable_name);
		m_BindingList.Insert(binding.GetWidgetName(), binding);
	}

	
	void OnPropertyChanged(string property_name)
	{
		//EditorLog.Trace(string.Format("Controller::NotifyPropertyChanged: %1", property_name));
		m_EditorViewList.Get(property_name).OnPropertyChanged();
		m_BindingList.Get(property_name).OnPropertyChanged();
	}
	
	
	void OnCollectionChanged(string property_name, CollectionChangedEventArgs args)
	{
		EditorLog.Trace(string.Format("Controller::OnCollectionChanged: %1 Action: %2", property_name, args.param2));
		m_EditorViewList.Get(property_name).OnCollectionChanged(args);	
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
				_GetTypeResult = type;
				return;
			}		
		}
		
		_GetTypeFromWidgetSource(source.GetChildren());
		_GetTypeFromWidgetSource(source.GetSibling());
	}
	
	ref EditorViewBase GetEditorView(string property_name)
	{
		Print(m_EditorViewList.Count());
		return m_EditorViewList.Get(property_name);
	}
	

	
	static WidgetSource GetWidgetSource()
	{
#ifdef COMPONENT_SYSTEM
		ResourceBrowser m_Module = Workbench.GetModule("ResourceManager");
		return m_Module.GetContainer();
#endif
	}

	
	ref map<string, typename> GetVariableHashMap() { return m_ModelHashMap; }
	typename GetVariableType(string var_name) { return m_ModelHashMap.Get(var_name); }
	
	
	bool OnClick(Widget w, int x, int y, bool button) { return true; }
	
	void DebugPrint()
	{
		EditorLog.Debug("Controller::DebugPrint: " + m_ControllerWidget.GetName());
		foreach (string name, ref EditorView view: m_EditorViewList)
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








/*

this is broke idk WHY
modded class EditorViewOptionsCallback
{
	
	protected ResourceBrowser m_Module;
	protected Controller m_Controller;
	
	ref array<ref ParamEnum> ResourceSearch()
	{

		m_Module = Workbench.GetModule("ResourceManager");
		WidgetSource widget = m_Module.GetContainer();
		
		if (ControllerHashMap != null) {
			m_Controller = ControllerHashMap.Get(Controller.GetTypeFromWidgetSource(widget));
			
			if (m_Controller != null) {
				Print("Enumerating views...");
				
				EditorViewHashMap view_list = m_Controller.GetEditorViewList();
				int i;
				foreach (string property_name, EditorView view: view_list) {
					param_enums.Insert(ParamEnum(property_name, i.ToString()));
					i++;
				}
	
				// success
				return param_enums;
			}
		}
		
		// Fail condition
		Workbench.Dialog("oof!", "Controller not found!");
		return null;
	}	
};
*/

