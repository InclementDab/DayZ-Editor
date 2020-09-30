class PrefabBaseController<Class TValue>: Controller
{
	string Caption;
	TValue Value;
	
	override void PropertyChanged(string property_name)
	{
		if (GetParent() && GetParent().IsInherited(PrefabBase)) {
			g_Script.Call(GetParent(), "PrefabPropertyChanged", property_name);
		}
	}
}


class PrefabBase<Class TValue>: ScriptView
{
	protected PrefabBaseController<TValue> m_PrefabBaseController;
	protected Controller m_BindingContext;
	protected string m_BindingName;
	
	void PrefabBase(Widget parent = null, string caption = "", Controller binding_context = null, string binding_name = "")
	{
		m_BindingName = binding_name;
		m_BindingContext = binding_context;
	
		Class.CastTo(m_PrefabBaseController, m_Controller);
		m_PrefabBaseController.Caption = caption;
		m_PrefabBaseController.NotifyPropertyChanged("Caption");
		
		
		// todo once you remove the Widget parent = null crap
		//m_PrefabBaseController.Value = default_value;
		//m_PrefabBaseController.NotifyPropertyChanged("Value");
	}
	
	void PrefabPropertyChanged(string property_name);

	override typename GetControllerType() {
		return (new PrefabBaseController<TValue>()).Type();
	}
}