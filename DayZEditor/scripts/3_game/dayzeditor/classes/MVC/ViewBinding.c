
typedef Param2<string, int> ViewBindingAction;


class ViewBindingActionController
{
	ref array<ref ViewBindingAction> ViewBindingActionList = {
		new ViewBindingAction("visible", WidgetFlags.VISIBLE),
		new ViewBindingAction("disabled", WidgetFlags.DISABLED),
		new ViewBindingAction("clipchildren", WidgetFlags.CLIPCHILDREN)
		
	};
	
	
	
	WidgetFlags Get(string target)
	{
		foreach (ref ViewBindingAction action: ViewBindingActionList)
			if (action.param1 == target)
				return action.param2;
		
		return 0;
	}
}







class ViewBinding: Managed
{
	
	reference string ControllerWidget;
	string GetViewModelWidgetName() { return ControllerWidget; }
	
	// Comma seperated values of Widget properties to bind
	reference string binding_name;
	string GetBindingName() { return binding_name; } 
	
	reference string variable_name;
	string GetVariableName() { return variable_name; } 
	
	reference bool invert_variable;
	
	reference int variable_index;
	
	private Widget m_LayoutRoot;
	string GetWidgetName() { return m_LayoutRoot.GetName(); }
	
	private Widget m_ViewModelWidget;
	
	private Controller m_ViewModel;
	
	private ref array<string> binding_types = { "visible", "disabled", "clipchildren", "inheritalpha", "ignorepointer", "keepsafezone" };
	private ref array<string> bound_types = new array<string>();
	
	
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("ViewBinding::OnWidgetScriptInit");
		if (ControllerWidget == string.Empty) return;
		m_LayoutRoot = w;
		 
		binding_name.Split(",", bound_types);
		foreach (string bind: bound_types) {
			bind.TrimInPlace();
			if (binding_types.Find(bind) == -1) {
				Error(string.Format("Invalid Bind Value: %1", bind));
				return;
			}
		}
		
		m_ViewModelWidget = GetWidgetRoot(m_LayoutRoot).FindAnyWidget(ControllerWidget);
		
		if (!m_ViewModelWidget) {
			Error(string.Format("ViewModel Widget not found! %1", ControllerWidget));
			return;
		}
		
		m_ViewModelWidget.GetScript(m_ViewModel);
		
		if (!m_ViewModel) {
			Error(string.Format("%1 Could not find ViewModel: %2", m_LayoutRoot.GetName(), ControllerWidget));
			return;
		}
		
		
		m_ViewModel.InsertBinding(this);
		
	}
	
	void OnPropertyChanged()
	{
		ViewBindingActionController controller();
		foreach (string binding: bound_types) {
		
			bool _BindingParamValue;
			EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _BindingParamValue);
	
			if (_BindingParamValue)
				m_LayoutRoot.SetFlags(controller.Get(binding));
			else 
				m_LayoutRoot.ClearFlags(controller.Get(binding));		
		
		}
	}	
}


