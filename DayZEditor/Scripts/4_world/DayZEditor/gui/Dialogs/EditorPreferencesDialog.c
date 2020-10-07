
class EditorPreferencesDialogController: DialogBaseController
{
	DropdownListPrefabItem SelectedLogLevel;
	
	float auto_save;
	
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			
			case "auto_save": {
				GetEditor().GetSettings().AutoSaveTimer = auto_save;
				break;
			}
			
		}
		
	}
	
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
		
		general_group.Insert(new EditBoxNumberPrefab("Auto Save Delay", m_Controller, "auto_save", GetEditor().GetSettings().AutoSaveTimer.ToString()));
		
		AddContent(general_group);
		
		AddButton(DialogResult.OK);
		AddButton(DialogResult.Cancel);
	}
	
	override typename GetControllerType() {
		return EditorPreferencesDialogController;
	}
}