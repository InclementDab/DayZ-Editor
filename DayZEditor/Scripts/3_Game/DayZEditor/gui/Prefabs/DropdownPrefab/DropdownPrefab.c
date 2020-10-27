
class DropdownListPrefabController: PrefabBaseController<DropdownListPrefabItem>
{
	ref ObservableCollection<ref DropdownListPrefabItem> DropdownElementList = new ObservableCollection<ref DropdownListPrefabItem>(this);
	
	void ~DropdownListPrefabController()
	{
		delete DropdownElementList;
	}
}

class DropdownListPrefab: PrefabBase<DropdownListPrefabItem>
{
	DropdownListPrefabController m_DropdownListController;

	private WrapSpacerWidget DropdownWrapper;
	
	void DropdownListPrefab(string caption, Controller binding_context, string binding_name, DropdownListPrefabItem default_value = DEFAULT_VALUE)
	{
		Class.CastTo(m_DropdownListController, m_Controller);
	}
	
	void InsertItem(string item_text, Class user_data = null)
	{
		InsertItem(new DropdownListPrefabItem(item_text, user_data));
	}
	
	void InsertItem(DropdownListPrefabItem element)
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