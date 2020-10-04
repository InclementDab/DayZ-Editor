
class DropdownElementPrefabController: Controller
{
	string Text;
	Class UserData;
}

class DropdownElementPrefab: ScriptViewTemplate<DropdownElementPrefabController>
{
	void DropdownElementPrefab(string text, Class user_data = null)
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

class DropdownListPrefabController: PrefabBaseController<DropdownElementPrefab>
{
	ref ObservableCollection<ref DropdownElementPrefab> DropdownElementList = new ObservableCollection<ref DropdownElementPrefab>("DropdownElementList", this);
	
	void ~DropdownListPrefabController()
	{
		delete DropdownElementList;
	}
}


class DropdownListPrefab: PrefabBase<DropdownElementPrefab>
{
	DropdownListPrefabController m_DropdownListController;

	private WrapSpacerWidget DropdownWrapper;
	
	void DropdownListPrefab(string caption, Controller binding_context, string binding_name, DropdownElementPrefab default_value = DEFAULT_VALUE)
	{
		Class.CastTo(m_DropdownListController, m_Controller);
	}
	
	
	void InsertItem(string item_text)
	{
		InsertItem(new DropdownElementPrefab(item_text));
	}
	
	void InsertItem(DropdownElementPrefab element)
	{
		element.SetParent(this);
		m_DropdownListController.DropdownElementList.Insert(element);
	}
		
	bool DropdownPrefabExecute(ButtonCommandArgs args)
	{
		DropdownWrapper.Show(!DropdownWrapper.IsVisible());
		return true;
	}
	
	bool DropdownElementExecute(ButtonCommandArgs args)
	{
		for (int i = 0; i < m_DropdownListController.DropdownElementList.Count(); i++) {
			if (m_DropdownListController.DropdownElementList[i].GetLayoutRoot().FindAnyWidget(args.Source.GetName()) == args.Source) {
				m_DropdownListController.Value = m_DropdownListController.DropdownElementList[i];
				m_DropdownListController.NotifyPropertyChanged("Value");
				DropdownWrapper.Show(false);
				return true;
			}
		}

		return true;
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/prefabs/Dropdown/DropdownPrefab.layout";
	}
	
	override typename GetControllerType() {
		return DropdownListPrefabController;
	}
}