class GroupPrefab: PrefabBase<ObservableCollection<ref ScriptView>>
{
	ref ObservableCollection<ref ScriptView> collection_ref;
	
	protected WrapSpacerWidget EditorDialogOptionContent;
	
	void GroupPrefab(string caption, Class binding_context, string binding_name, ObservableCollection<ref ScriptView> default_value = DEFAULT_VALUE)
	{
		collection_ref = new ObservableCollection<ref ScriptView>(m_PrefabBaseController);
		m_PrefabBaseController.Value = collection_ref;
	}
	
	void ~GroupPrefab()
	{
		delete collection_ref;
	}
	
	void Insert(ScriptView content)
	{
		m_PrefabBaseController.Value.Insert(content);
	}
	
	bool GroupCollapseExecute(ButtonCommandArgs args)
	{
		Open(!args.GetButtonState()); 
		return true;		
	}
	
	bool IsOpen()
	{
		return EditorDialogOptionContent.IsVisible();
	}
	
	void Open(bool state)
	{
		EditorDialogOptionContent.Show(state);
	}
	
	// Inefficient but unsure else how to propagate updates to all children
	void Update()
	{
		for (int i = 0; i < collection_ref.Count(); i++) {
			collection_ref[i].GetController().NotifyPropertyChanged();
		}
	}
	
	override void PrefabPropertyChanged(string property_name)
	{
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/prefabs/GroupPrefab.layout";
	}
}