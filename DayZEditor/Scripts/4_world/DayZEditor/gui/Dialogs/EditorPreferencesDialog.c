
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
			
			case "SelectedLogLevel": {
				EditorLog.CurrentLogLevel = Param1<LogLevel>.Cast(SelectedLogLevel.GetTemplateController().UserData).param1;
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
		
		DropdownListPrefabItem trace_item = new DropdownListPrefabItem("Trace", new Param1<LogLevel>(LogLevel.TRACE));
		DropdownListPrefab log_level = new DropdownListPrefab("Log Level", m_Controller, "SelectedLogLevel", trace_item);
		log_level.InsertItem(trace_item);
		log_level.InsertItem("Debug", new Param1<LogLevel>(LogLevel.DEBUG));
		log_level.InsertItem("Info", new Param1<LogLevel>(LogLevel.INFO));
		log_level.InsertItem("Warning", new Param1<LogLevel>(LogLevel.WARNING));
		log_level.InsertItem("Error", new Param1<LogLevel>(LogLevel.ERROR));
		general_group.Insert(log_level);
		
		general_group.Insert(new EditBoxNumberPrefab("Auto Save", m_Controller, "auto_save", GetEditor().GetSettings().AutoSaveTimer.ToString()));
		
		AddContent(general_group);
		
		AddButton(DialogResult.OK);
		AddButton(DialogResult.Cancel);
	}
	
	override typename GetControllerType() {
		return EditorPreferencesDialogController;
	}
}