class DropdownListPrefabItemController<Class TValue>: ViewController
{
	string Text;
	TValue UserData;
}

class DropdownListPrefabItemBase: ScriptView
{
	string GetText()
	{
		return string.Empty;
	}
}

class DropdownListPrefabItem<Class TValue>: DropdownListPrefabItemBase
{
	protected DropdownListPrefabItemController<TValue> m_DropdownListPrefabItemController;
	
	void DropdownListPrefabItem(string text, TValue user_data)
	{
		Class.CastTo(m_DropdownListPrefabItemController, m_Controller);
		
		m_DropdownListPrefabItemController.Text = text;
		m_DropdownListPrefabItemController.NotifyPropertyChanged("Text");
		
		m_DropdownListPrefabItemController.UserData = user_data;
		m_DropdownListPrefabItemController.NotifyPropertyChanged("UserData");
	}
	
	bool DropdownElementExecute(ButtonCommandArgs args)
	{
		DropdownListPrefab<TValue> dropdown = DropdownListPrefab<TValue>.Cast(GetParent());
		if (dropdown) {
			dropdown.DropdownElementExecute(this);
		}
		
		return true;
	}
	
	override string GetText()
	{
		return m_DropdownListPrefabItemController.Text;
	}
	
	TValue GetValue()
	{
		return m_DropdownListPrefabItemController.UserData;
	}
		
	override typename GetControllerType()
	{
		return (new DropdownListPrefabItemController<TValue>()).Type();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/prefabs/Dropdown/DropdownElementPrefab.layout";
	}
}