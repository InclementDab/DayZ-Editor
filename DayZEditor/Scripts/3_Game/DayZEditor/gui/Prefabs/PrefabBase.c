class PrefabBaseController<Class TValue>: ViewController
{
	string Caption;
	TValue Value;
	TValue CalculatedValue; // Used for things like SliderWidget output
	
	override void PropertyChanged(string property_name)
	{
		if (GetParent() && GetParent().IsInherited(PrefabBase)) {
			g_Script.Call(GetParent(), "PrefabPropertyChanged", property_name);
		}
	}
}

class PrefabBase<Class TValue>: ScriptView
{
	private static const TValue DEFAULT_VALUE;
	
	protected PrefabBaseController<TValue> m_PrefabBaseController;
	protected Class m_BindingContext;
	protected string m_BindingName;
	
	// With Direct Binding, I think we can depreciate the default_value
	void PrefabBase(string caption, Class binding_context, string binding_name, TValue default_value = DEFAULT_VALUE)
	{
		m_BindingName = binding_name;
		m_BindingContext = binding_context;
	
		if (default_value == DEFAULT_VALUE) {
			EnScript.GetClassVar(m_BindingContext, m_BindingName, 0, default_value);
		}
		
		Class.CastTo(m_PrefabBaseController, m_Controller);
		m_PrefabBaseController.Caption = caption;
		m_PrefabBaseController.NotifyPropertyChanged("Caption", false);
		
		m_PrefabBaseController.Value = default_value;
		m_PrefabBaseController.NotifyPropertyChanged("Value", false);
	}
	
	void PrefabPropertyChanged(string property_name)
	{
		EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, m_PrefabBaseController.Value);		
		g_Script.CallFunction(m_BindingContext, "PropertyChanged", null, m_BindingName);
	}

	override typename GetControllerType() 
	{
		return (new PrefabBaseController<TValue>()).Type();
	}
	
	PrefabBaseController<TValue> GetPrefabController() 
	{
		return m_PrefabBaseController;
	}
}