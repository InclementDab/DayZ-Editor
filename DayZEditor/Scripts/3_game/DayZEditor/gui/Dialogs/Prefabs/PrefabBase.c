class PrefabBaseController: Controller
{
	string Caption;
	
	override void PropertyChanged(string property_name)
	{
		if (GetParent() && GetParent().IsInherited(PrefabBase)) {
			PrefabBase.Cast(GetParent()).PrefabPropertyChanged(property_name);
		}
	}
}


class PrefabBase: ScriptView
{
	protected PrefabBaseController m_PrefabBaseController;
	
	protected Controller m_BindingContext;
	protected string m_BindingName;
	
	void PrefabBase(Widget parent = null, string caption = "", Controller binding_context = null, string binding_name = "")
	{
		m_BindingName = binding_name;
		m_BindingContext = binding_context;
		
		m_PrefabBaseController = PrefabBaseController.Cast(GetController());
		m_PrefabBaseController.Caption = caption;
		m_PrefabBaseController.NotifyPropertyChanged("Caption");
	}
	
	void PrefabPropertyChanged(string property_name);
}