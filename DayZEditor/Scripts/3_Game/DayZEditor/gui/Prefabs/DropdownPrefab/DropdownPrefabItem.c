class DropdownListPrefabItemController: Controller
{
	string Text;
	Param UserData;
}

class DropdownListPrefabItem: ScriptViewTemplate<DropdownListPrefabItemController>
{
	void DropdownListPrefabItem(string text, Param user_data)
	{
		m_TemplateController.Text = text;
		m_TemplateController.NotifyPropertyChanged("Text");
		
		m_TemplateController.UserData = user_data;
		m_TemplateController.NotifyPropertyChanged("UserData");
	}
	
	Param GetValue()
	{
		return m_TemplateController.UserData;
	}
		
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/prefabs/Dropdown/DropdownElementPrefab.layout";
	}
}