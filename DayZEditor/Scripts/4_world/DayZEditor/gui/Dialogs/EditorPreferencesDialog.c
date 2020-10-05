
class EditorPreferencesDialogController: DialogBaseController
{
	DropdownListPrefabItem SelectedLogLevel;
}

class EditorPreferencesDialog: EditorDialogBase
{
	protected EditorPreferencesDialogController m_EditorPreferencesDialogController;
	
	void EditorPreferencesDialog(string title)
	{
		m_EditorPreferencesDialogController = EditorPreferencesDialogController.Cast(m_Controller);
		GroupPrefab general_group = new GroupPrefab("General", m_Controller, string.Empty);
		
		
		DropdownListPrefab log_level = new DropdownListPrefab("Log Level", m_Controller, "SelectedLogLevel");
		log_level.InsertItem("Trace");
		log_level.InsertItem("Debug");
		general_group.Insert(log_level);
		
		AddContent(general_group);
		
		AddButton(DialogResult.OK);
		AddButton(DialogResult.Cancel);
	}
	
	override typename GetControllerType() {
		return EditorPreferencesDialogController;
	}
}