
class DropdownListPrefabController<Class TValue>: Controller
{
	ref ObservableCollection<ref DropdownListPrefabItem> DropdownElementList = new ObservableCollection<ref DropdownListPrefabItem>(this);
	
	string Caption;
	TValue Value;
	TValue CalculatedValue; // Used for things like SliderWidget output
	
	void ~DropdownListPrefabController()
	{
		delete DropdownElementList;
	}
	
	override void PropertyChanged(string property_name)
	{
		if (GetParent() && GetParent().IsInherited(PrefabBase)) {
			g_Script.Call(GetParent(), "PrefabPropertyChanged", property_name);
		}
	}
}

class DropdownListPrefab<Class TValue>: ScriptView
{
	private WrapSpacerWidget DropdownWrapper;
	
	private static const TValue DEFAULT_VALUE;
	
	protected TValue m_DefaultAssignedValue;
	
	protected DropdownListPrefabController m_DropdownPrefabController;
	
	protected Class m_BindingContext;
	protected string m_BindingName;
	
	// With Direct Binding, I think we can depreciate the default_value
	void DropdownListPrefab(string caption, Class binding_context, string binding_name, TValue default_value = DEFAULT_VALUE)
	{
		m_BindingName = binding_name;
		m_BindingContext = binding_context;
			
		if (default_value == DEFAULT_VALUE) {
			EnScript.GetClassVar(m_BindingContext, m_BindingName, 0, default_value);
		}
		
		Class.CastTo(m_DropdownPrefabController, m_Controller);
		m_DropdownPrefabController.Caption = caption;
		m_DropdownPrefabController.NotifyPropertyChanged("Caption", false);
		
		m_DefaultAssignedValue = default_value;
		//m_DropdownPrefabController.Value = new Param1<ref TValue>(default_value);
		//m_DropdownPrefabController.NotifyPropertyChanged("Value", false);
	}
	
	DropdownListPrefabItem InsertItem(string item_text, TValue user_data)
	{
		return InsertItem(new DropdownListPrefabItem(item_text, new Param1<TValue>(user_data)));
	}
	
	DropdownListPrefabItem InsertItem(DropdownListPrefabItem element)
	{
		element.SetParent(this);
		m_DropdownPrefabController.DropdownElementList.Insert(element);
		return element;
	}
		
	bool DropdownPrefabExecute(ButtonCommandArgs args)
	{
		DropdownWrapper.Show(!DropdownWrapper.IsVisible());
		return true;
	}
	
	bool DropdownElementExecute(ButtonCommandArgs args)
	{
		for (int i = 0; i < m_DropdownPrefabController.DropdownElementList.Count(); i++) {
			if (m_DropdownPrefabController.DropdownElementList[i].GetLayoutRoot().FindAnyWidget(args.Source.GetName()) == args.Source) {
				m_DropdownPrefabController.Value = m_DropdownPrefabController.DropdownElementList[i].GetValue();
				m_DropdownPrefabController.NotifyPropertyChanged("Value");
				DropdownWrapper.Show(false);
				return true;
			}
		}

		return true;
	}
	
	DropdownListPrefabItem GetListItem(TValue value)
	{
		for (int i = 0; i < m_DropdownPrefabController.DropdownElementList.Count(); i++) {
			Param1<TValue> list_item_value = Param1<TValue>.Cast(m_DropdownPrefabController.DropdownElementList[i].GetValue());
			if (!list_item_value) {
				continue;
			}
			
			if (list_item_value.param1 == value) {
				return m_DropdownPrefabController.DropdownElementList[i];
			}
		}
		
		return null;
	}
	
	void PrefabPropertyChanged(string property_name)
	{
		EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, m_DropdownPrefabController.Value);		
		g_Script.CallFunction(m_BindingContext, "PropertyChanged", null, m_BindingName);
	}
	
	override typename GetControllerType() 
	{
		return (new DropdownListPrefabController<TValue>()).Type();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/prefabs/Dropdown/DropdownPrefab.layout";
	}
}