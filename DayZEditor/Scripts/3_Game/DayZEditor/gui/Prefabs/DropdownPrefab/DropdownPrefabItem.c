class DropdownListPrefabItemController: Controller
{
	string Text;
	Class UserData;
}

class DropdownListPrefabItem: ScriptViewTemplate<DropdownListPrefabItemController>
{
	void DropdownListPrefabItem(string text, Class user_data = null)
	{
		m_TemplateController.Text = text;
		m_TemplateController.NotifyPropertyChanged("Text");
		
		m_TemplateController.UserData = user_data;
		m_TemplateController.NotifyPropertyChanged("UserData");
	}
		
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/prefabs/Dropdown/DropdownElementPrefab.layout";
	}
}