class GroupPrefab: PrefabBase<ObservableCollection<ref ScriptedViewBase>>
{
	ref ObservableCollection<ref ScriptedViewBase> collection_ref;
	
	protected WrapSpacerWidget EditorDialogOptionContent;
	
	void GroupPrefab(string caption, Controller binding_context, string binding_name, ObservableCollection<ref ScriptedViewBase> default_value = DEFAULT_VALUE)
	{
		collection_ref = new ObservableCollection<ref ScriptedViewBase>(m_PrefabBaseController);
		m_PrefabBaseController.Value = collection_ref;
	}
	
	void ~GroupPrefab()
	{
		delete collection_ref;
	}
	
	void Insert(ScriptedViewBase content)
	{
		m_PrefabBaseController.Value.Insert(content);
	}
	
	bool GroupCollapseExecute(ButtonCommandArgs args)
	{
		EditorDialogOptionContent.Show(!args.GetButtonState()); 
		return true;		
	}
	
	override void PrefabPropertyChanged(string property_name)
	{
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/prefabs/GroupPrefab.layout";
	}
}