class GroupPrefab: PrefabBase<ref ObservableCollection<ref ScriptedViewBase>>
{
	void Insert(ScriptedViewBase content)
	{
		m_PrefabBaseController.Value.Insert(content);
	}
	
	override void PrefabPropertyChanged(string property_name)
	{
		//EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, m_PrefabBaseController.Value);
		//m_BindingContext.PropertyChanged(m_BindingName);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/prefabs/GroupPrefab.layout";
	}
}