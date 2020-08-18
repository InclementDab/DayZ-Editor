
typedef ref array<ref EditorView> EditorViewSet;

// Inherit this class, then put that class into ScriptClass for your View Model Widget
class ViewModel: Managed
{
	// Widget Data
	protected Widget m_LayoutRoot;
	protected Widget m_ViewModelWidget;
	
	// Hashed Variable Data
	protected ref map<string, typename> m_ModelHashMap;
	
	// View List
	protected ref map<string, ref EditorViewSet> m_ViewList;
	protected ref map<string, ref ViewBinding> m_BindingList;
	
	void ViewModel() { EditorLog.Trace("ViewModel"); }
	void ~ViewModel() { EditorLog.Trace("~ViewModel"); }
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("ViewModel::OnWidgetScriptInit");	
		m_ViewModelWidget = w;
		m_LayoutRoot = GetWidgetRoot(m_ViewModelWidget);
		
		m_ModelHashMap = new map<string, typename>();
		m_ViewList = new map<string, ref EditorViewSet>();
		m_BindingList = new map<string, ref ViewBinding>();
		
		typename vtype = Type();
		int vcnt = vtype.GetVariableCount();
		for (int i = 0; i < vcnt; i++)
			m_ModelHashMap.Insert(vtype.GetVariableName(i), vtype.GetVariableType(i));		
		
		
		NotifyOnPropertyChanged(OnPropertyChanged);
		NotifyOnCollectionChanged(OnCollectionChanged);
	}
	
	void InsertView(ref EditorView view)
	{
		string variable_name = view.GetVariableName();
		EditorLog.Trace("ViewModel::InsertView: " + variable_name);
		
		if (m_ViewList.Get(variable_name)) {
			EditorViewSet view_set = m_ViewList.Get(variable_name);
			view_set.Insert(view);
		} else {
			view_set = new EditorViewSet();	
			view_set.Insert(view);
			m_ViewList.Insert(variable_name, view_set);
		}
	}
	
	void InsertBinding(ViewBinding binding)
	{
		string variable_name = binding.GetVariableName();
		EditorLog.Trace("ViewModel::InsertBinding: " + variable_name);
		
		m_BindingList.Insert(binding.GetWidgetName(), binding);
	}
	
	
	void OnPropertyChanged(string property_name)
	{
		//EditorLog.Trace(string.Format("ViewModel::NotifyPropertyChanged: %1", property_name));
		
		// for whatever reason. this action returns null every other time....
		EditorViewSet view_set = m_ViewList.Get(property_name);
		if (view_set == null) {
			//Error(string.Format("Invalid Property Name: %1 - View not found", property_name));
			return;
		}
		
		foreach (EditorView view: view_set) {
			view.OnPropertyChanged();
		}
		
		foreach (ViewBinding binding: m_BindingList) {
			if (binding.GetVariableName() == property_name) {
				binding.OnPropertyChanged();
			}
		}
		
		
	}
	
	
	void OnCollectionChanged(string property_name, CollectionChangedEventArgs args)
	{
		EditorLog.Trace(string.Format("ViewModel::OnCollectionChanged: %1 Action: %2", property_name, args.param2));
		EditorViewSet view_set = m_ViewList.Get(property_name);
		if (view_set == null) {
			EditorLog.Debug(string.Format("ViewModel::OnCollectionChanged: View was null! %1", property_name));
			//Error(string.Format("Invalid Collection Name: %1 - View not found", property_name));
			return;
		}
		
		foreach (EditorView view: view_set) {
			view.OnCollectionChanged(args);
		}
		
	}

	
	ref map<string, typename> GetVariableHashMap() { return m_ModelHashMap; }
	typename GetVariableType(string var_name) { return m_ModelHashMap.Get(var_name); }
	
	
	bool OnClick(Widget w, int x, int y, bool button) { return true; }
	
	void DebugPrint()
	{
		EditorLog.Debug("ViewModel::DebugPrint: " + m_ViewModelWidget.GetName());
		foreach (string name, ref EditorViewSet view_set: m_ViewList)
			foreach (EditorView view: view_set)
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