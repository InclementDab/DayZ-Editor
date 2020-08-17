
// abstract
class ViewModel: Managed
{
	// Just stored for debug
	protected Widget m_LayoutRoot;
	
	// Hashed Variable Data
	protected ref map<string, typename> m_ModelHashMap;
	
	// View List
	protected ref map<string, ref EditorView> m_ViewList;
	
	void ViewModel() { EditorLog.Trace("ViewModel"); }
	void ~ViewModel() { EditorLog.Trace("~ViewModel"); }
	
		
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("ViewModel::OnWidgetScriptInit");	
		m_LayoutRoot = w;
		
		m_ModelHashMap = new map<string, typename>();
		m_ViewList = new map<string, ref EditorView>();

		typename vtype = Type();
		int vcnt = vtype.GetVariableCount();
		for (int i = 0; i < vcnt; i++)
			m_ModelHashMap.Insert(vtype.GetVariableName(i), vtype.GetVariableType(i));		
	}
	
	void InsertView(string variable_name, ref EditorView view)
	{
		EditorLog.Trace("ViewModel::InsertView: " + variable_name);
		m_ViewList.Insert(variable_name, view);
	}
	
	void DebugPrint()
	{
		EditorLog.Debug("ViewModel::DebugPrint: " + m_LayoutRoot.GetName());
		foreach (string name, ref EditorView view: m_ViewList)
			view.DebugPrint();
	}
	
	// property_name = name of variable being changed
	ref ScriptInvoker PropertyChanged = new ScriptInvoker();
	void NotifyOnPropertyChanged(func action)
	{	
		if (PropertyChanged == null)
			PropertyChanged = new ScriptInvoker();
		
		PropertyChanged.Insert(action);
	}
	
	void NotifyPropertyChanged(string property_name = "") 
	{
		EditorLog.Trace("ViewModel::NotifyPropertyChanged: " + property_name);
		if (property_name == string.Empty) {
			foreach (ref EditorView view: m_ViewList)
				view.UpdateView();
			return;
		}
		
			
		PropertyChanged.Invoke(property_name);
	}

	
	ref map<string, typename> GetVariableHashMap() { return m_ModelHashMap; }
	typename GetVariableType(string var_name) { return m_ModelHashMap.Get(var_name); }
	
	bool OnClick(Widget w, int x, int y, bool button) { return true; }
}