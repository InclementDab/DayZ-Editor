
class ViewBinding: Managed
{
	
	reference string view_model_widget;
	string GetViewModelWidgetName() { return view_model_widget; }
	
	// Comma seperated values of Widget properties to bind
	reference string binding_name;
	string GetBindingName() { return binding_name; } 
	
	reference string variable_name;
	string GetVariableName() { return variable_name; } 
	
	reference int variable_index;
	
	private Widget m_LayoutRoot;
	string GetWidgetName() { return m_LayoutRoot.GetName(); }
	
	private Widget m_ViewModelWidget;
	
	private ViewModel m_ViewModel;
	
	private ref array<string> binding_types = { "visible", "disabled", "clipchildren", "inheritalpha", "ignorepointer", "keepsafezone" };
	private ref array<string> bound_types = new array<string>();
	
	
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("ViewBinding::OnWidgetScriptInit");
		if (view_model_widget == string.Empty) return;
		m_LayoutRoot = w;
		 
		binding_name.Split(",", bound_types);
		foreach (string bind: bound_types) {
			bind.TrimInPlace();
			if (binding_types.Find(bind) == -1) {
				Error(string.Format("Invalid Bind Value: %1", bind));
				return;
			}
		}
		
		m_ViewModelWidget = GetWidgetRoot(m_LayoutRoot).FindAnyWidget(view_model_widget);
		
		if (!m_ViewModelWidget) {
			Error(string.Format("ViewModel Widget not found! %1", view_model_widget));
			return;
		}
		
		m_ViewModelWidget.GetScript(m_ViewModel);
		
		if (!m_ViewModel) {
			Error(string.Format("%1 Could not find ViewModel: %2", m_LayoutRoot.GetName(), view_model_widget));
			return;
		}
		
		
		m_ViewModel.InsertBinding(this);
		
	}
	
	void OnPropertyChanged()
	{
		foreach (string binding: bound_types) {
			Print(m_LayoutRoot.GetName());
			switch (binding) {
				
				case "visible": {
					bool _TextWidgetDataB;
					EnScript.GetClassVar(m_ViewModel, variable_name, variable_index, _TextWidgetDataB);
					m_LayoutRoot.Show(_TextWidgetDataB);
					m_LayoutRoot.Update();
					break;
				}
				
			}
			
		}
	}
	
}


