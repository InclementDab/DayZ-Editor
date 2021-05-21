class DropdownListPrefabItemController: Controller
{
	string Text;
	ref Param UserData;
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
	
	string GetText()
	{
		return m_TemplateController.Text;
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