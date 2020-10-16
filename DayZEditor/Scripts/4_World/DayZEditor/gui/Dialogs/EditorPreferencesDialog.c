
class EditorPreferencesDialogController: DialogBaseController
{
	ref DropdownListPrefabItem SelectedLogLevel;
	
	float auto_save;
	
	float view_distance;
	float object_view_distance;
	
	bool modal_dialogs;
	bool debug_mode;
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			
			case "auto_save": {
				GetEditor().GetSettings().AutoSaveTimer = auto_save;
				break;
			}
			
			case "SelectedLogLevel": {
				
				if (SelectedLogLevel.GetTemplateController() && SelectedLogLevel.GetTemplateController().UserData) {
					Param1<LogLevel> p = Param1<LogLevel>.Cast(SelectedLogLevel.GetTemplateController().UserData);
					if (p) {
						EditorLog.CurrentLogLevel = p.param1;
					}
				}
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
					
			case "modal_dialogs": {
				GetEditor().GetSettings().LockCameraDuringDialogs = modal_dialogs;
				break;
			}
			
			case "debug_mode": {
				GetEditor().GetSettings().DebugMode = debug_mode;
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
		
		GroupPrefab debug_group = new GroupPrefab("Editor", m_Controller, string.Empty);
		debug_group.Insert(new CheckBoxPrefab("Modal Dialogs", m_Controller, "modal_dialogs", GetEditor().GetSettings().LockCameraDuringDialogs));
		debug_group.Insert(new CheckBoxPrefab("Debug Mode", m_Controller, "debug_mode", GetEditor().GetSettings().DebugMode));
		
		AddContent(game_group);
		AddContent(debug_group);
		AddButton(DialogResult.OK);
		AddButton(DialogResult.Cancel);
	}
	
	override typename GetControllerType() {
		return EditorPreferencesDialogController;
	}
}