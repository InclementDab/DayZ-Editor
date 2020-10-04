
class DropdownElementPrefabController: Controller
{
	string Text;
}

class DropdownElementPrefab: ScriptViewTemplate<DropdownElementPrefabController>
{
	void DropdownElementPrefab(string text)
	{
		m_TemplateController.Text = text;
		m_TemplateController.NotifyPropertyChanged("Text");
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/prefabs/Dropdown/DropdownElementPrefab.layout";
	}
}


class DropdownListPrefab: PrefabBase<ref ObservableCollection<ref ScriptedViewBase>>
{
	ref ObservableCollection<ref ScriptedViewBase> collection_ref;
	
	private WrapSpacerWidget DropdownWrapper;
	
	void DropdownListPrefab(string caption, Controller binding_context, string binding_name, ObservableCollection<ref ScriptedViewBase> default_value = DEFAULT_VALUE)
	{
		Debug_Logging = true;
		collection_ref = new ref ObservableCollection<ref ScriptedViewBase>("Value", m_PrefabBaseController);
		m_PrefabBaseController.Value = collection_ref;
	}
	
	void InsertItem(string item_text)
	{
		InsertItem(new DropdownElementPrefab(item_text));
	}
	
	void InsertItem(DropdownElementPrefab element)
	{
		m_PrefabBaseController.Value.Insert(element);
		
		Print(m_PrefabBaseController.Value);
	}
	
	void Open(bool open)
	{
		DropdownWrapper.Show(open);
	}
	
	bool DropdownPrefabExecute(ButtonCommandArgs args)
	{
		Print(m_PrefabBaseController.Value.Count());
		Print("hi");
		DropdownWrapper.Show(!DropdownWrapper.IsVisible());		
		return true;
	}
	
		
	override void PrefabPropertyChanged(string property_name)
	{
		//EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, m_PrefabBaseController.Value);
		//m_BindingContext.PropertyChanged(m_BindingName);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/prefabs/Dropdown/DropdownPrefab.layout";
	}
	
}