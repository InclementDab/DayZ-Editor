
class EditorPreferencesDialog: EditorDialogBase
{	
	void EditorPreferencesDialog(string title)
	{
		GroupPrefab general_group = new GroupPrefab("General", GetEditor().Settings, string.Empty);
		
		DropdownListPrefab log_level = new DropdownListPrefab("Log Level", GetEditor().Settings, "SelectedLogLevel");
		log_level.InsertItem("Trace", new Param1<LogLevel>(LogLevel.TRACE));
		log_level.InsertItem("Debug", new Param1<LogLevel>(LogLevel.DEBUG));
		log_level.InsertItem("Info", new Param1<LogLevel>(LogLevel.INFO));
		log_level.InsertItem("Warning", new Param1<LogLevel>(LogLevel.WARNING));
		log_level.InsertItem("Error", new Param1<LogLevel>(LogLevel.ERROR));
		general_group.Insert(log_level);		
		general_group.Insert(new EditBoxNumberPrefab("Auto Save", GetEditor().Settings, "AutoSaveTimer"));
		
		
		GroupPrefab game_group = new GroupPrefab("Game", GetEditor().Settings, string.Empty);
		game_group.Insert(new SliderPrefab("View Distance", GetEditor().Settings, "ViewDistance", 0, 20000));
		game_group.Insert(new SliderPrefab("Object View Distance", GetEditor().Settings, "ObjectViewDistance", 0, 8000));
		
		GroupPrefab debug_group = new GroupPrefab("Editor", GetEditor().Settings, string.Empty);
		debug_group.Insert(new EditBoxPrefab("Loot Spawns File", GetEditor().Settings, "EditorProtoFile"));
		debug_group.Insert(new EditBoxPrefab("Brush File", GetEditor().Settings, "EditorBrushFile"));
		debug_group.Insert(new CheckBoxPrefab("Modal Dialogs", GetEditor().Settings, "LockCameraDuringDialogs"));
		debug_group.Insert(new CheckBoxPrefab("Show Bounding Boxes", GetEditor().Settings, "ShowBoundingBoxes"));
		debug_group.Insert(new CheckBoxPrefab("Debug Mode", GetEditor().Settings, "DebugMode"));
		
		AddContent(general_group);
		AddContent(game_group);
		AddContent(debug_group);
		AddButton(DialogResult.OK);
		AddButton("Close", DialogResult.Cancel);
	}
}