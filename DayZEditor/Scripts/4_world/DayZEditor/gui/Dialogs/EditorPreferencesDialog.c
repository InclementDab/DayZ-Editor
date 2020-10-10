
class EditorPreferencesDialogController: DialogBaseController
{
	DropdownListPrefabItem SelectedLogLevel;
	
	float auto_save;
	
	float view_distance;
	float object_view_distance;
	
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
			
			case "view_distance": {
				GetEditor().GetSettings().ViewDistance = view_distance;
				GetGame().GetWorld().SetViewDistance(view_distance);
				break;
			}
			
			case "object_view_distance": {
				GetEditor().GetSettings().ObjectViewDistance = view_distance;
				GetGame().GetWorld().SetObjectViewDistance(object_view_distance);
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
		
		GroupPrefab game_group = new GroupPrefab("Game", m_Controller, string.Empty);
		game_group.Insert(new SliderPrefab("View Distance", m_Controller, "view_distance", GetEditor().GetSettings().ViewDistance, 0, 20000));
		game_group.Insert(new SliderPrefab("Object View Distance", m_Controller, "object_view_distance", GetEditor().GetSettings().ObjectViewDistance, 0, 8000));
		
		AddContent(game_group);
		AddButton(DialogResult.OK);
		AddButton(DialogResult.Cancel);
	}
	
	override typename GetControllerType() {
		return EditorPreferencesDialogController;
	}
}