
typedef ref array<ref EditorView> TEditorViewSet;
static ref map<string, Controller> ControllerHashMap;


// Inherit this class, then put that class into ScriptClass for your View Model Widget
class Controller: ControllerBase
{
	// Widget Data
	protected Widget m_LayoutRoot;
	protected Widget m_ControllerWidget;
	
	// Hashed Variable Data
	protected ref map<string, typename> m_ModelHashMap;
	
	protected ref map<string, ref ViewBinding> m_BindingList;
	
	void Controller() { EditorLog.Trace("Controller"); }
	void ~Controller() { EditorLog.Trace("~Controller"); }
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("Controller::OnWidgetScriptInit");
		m_ControllerWidget = w;
		m_LayoutRoot = GetWidgetRoot(m_ControllerWidget);
		
		m_ModelHashMap = new map<string, typename>();
		m_BindingList = new map<string, ref ViewBinding>();

		
		typename vtype = Type();
		int vcnt = vtype.GetVariableCount();
		for (int i = 0; i < vcnt; i++)
			m_ModelHashMap.Insert(vtype.GetVariableName(i), vtype.GetVariableType(i));		
		
		if (ControllerHashMap == null) {
			ControllerHashMap = new map<string, Controller>();
		}
		
		ControllerHashMap.Insert(m_ControllerWidget.GetName(), this);
		
		if (ControllerBaseHashMap == null) {
			ControllerBaseHashMap = new map<string, ControllerBase>();
		}
		
		ControllerBaseHashMap.Insert(m_ControllerWidget.GetName(), this);
		
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
	
	override static void CreateControllerWidget(string name) 
	{
		WorkspaceWidget workspace = g_Game.GetWorkspace();
		Widget frame = workspace.CreateWidget(FrameWidgetTypeID, 0, 0, 0, 0, 0, 0, 0, workspace);
		frame.SetName(name);
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