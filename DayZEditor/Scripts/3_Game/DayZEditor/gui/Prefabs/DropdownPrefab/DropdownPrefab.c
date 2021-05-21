
class DropdownListPrefabController<Class TValue>: Controller
{
	ref ObservableCollection<ref DropdownListPrefabItem<TValue>> DropdownElementList = new ObservableCollection<ref DropdownListPrefabItem<TValue>>(this);
	
	string Caption;
	DropdownListPrefabItem<TValue> Value;
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
	
	private static const TValue EMPTY_VALUE;
	
	protected TValue m_DefaultValue;
	
	protected DropdownListPrefabController<TValue> m_DropdownPrefabController;
	
	protected Class m_BindingContext;
	protected string m_BindingName;
	
	// With Direct Binding, I think we can depreciate the default_value
	void DropdownListPrefab(string caption, Class binding_context, string binding_name, TValue default_value = EMPTY_VALUE)
	{
		m_BindingName = binding_name;
		m_BindingContext = binding_context;
			
		if (default_value == EMPTY_VALUE) {
			EnScript.GetClassVar(m_BindingContext, m_BindingName, 0, default_value);
		}
		
		Class.CastTo(m_DropdownPrefabController, m_Controller);
		m_DropdownPrefabController.Caption = caption;
		m_DropdownPrefabController.NotifyPropertyChanged("Caption", false);
		
		m_DefaultValue = default_value;
	}
	
	DropdownListPrefabItem<TValue> InsertItem(string item_text, TValue user_data)
	{
		return InsertItem(new DropdownListPrefabItem<TValue>(item_text, user_data));
	}
	
	DropdownListPrefabItem<TValue> InsertItem(DropdownListPrefabItem<TValue> element)
	{
		element.SetParent(this);
		m_DropdownPrefabController.DropdownElementList.Insert(element);
		
		if (GetListItem(m_DefaultValue) == element) {
			SetActiveListItem(element);
		}
		
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
				SetActiveListItem(m_DropdownPrefabController.DropdownElementList[i]);
				DropdownWrapper.Show(false);
				return true;
			}
		}

		return true;
	}
	
	void SetActiveListItem(DropdownListPrefabItem<TValue> item)
	{
		Print(item);
		m_DropdownPrefabController.Value = item;
		m_DropdownPrefabController.NotifyPropertyChanged("Value");
	}
	
	DropdownListPrefabItem<TValue> GetListItem(TValue value)
	{
		for (int i = 0; i < m_DropdownPrefabController.DropdownElementList.Count(); i++) {
			TValue list_item_value = m_DropdownPrefabController.DropdownElementList[i].GetValue();
			if (!list_item_value) {
				continue;
			}
			
			if (list_item_value == value) {
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