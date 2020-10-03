
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
		return "DayZEditor/gui/Layouts/prefabs/DropdownElementPrefab.layout";
	}
}


class DropdownListPrefab: PrefabBase<ref ObservableCollection<ref DropdownElementPrefab>>
{
	autoptr ObservableCollection<ref DropdownElementPrefab> collection_ref;
	
	void DropdownListPrefab(string caption, Controller binding_context, string binding_name, ObservableCollection<ref DropdownElementPrefab> default_value = DEFAULT_VALUE)
	{
		collection_ref = new ref ObservableCollection<ref DropdownElementPrefab>("Value", m_PrefabBaseController);
		m_PrefabBaseController.Value = collection_ref;
	}
	
	void InsertItem(string item_text)
	{
		InsertItem(new DropdownElementPrefab(item_text));
	}
	
	void InsertItem(DropdownElementPrefab element)
	{
		m_PrefabBaseController.Value.Insert(element);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/prefabs/DropdownPrefab.layout";
	}
	
}