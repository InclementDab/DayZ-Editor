
class EditorPreferencesDialog: EditorDialogBase
{	
	void EditorPreferencesDialog(string title)
	{
		GroupPrefab general_group = new GroupPrefab("General", GetEditor().Settings, string.Empty);
		
		DropdownListPrefabItem trace_item = new DropdownListPrefabItem("Trace", new Param1<LogLevel>(LogLevel.TRACE));
		DropdownListPrefab log_level = new DropdownListPrefab("Log Level", GetEditor().Settings, "SelectedLogLevel", trace_item);
		log_level.InsertItem(trace_item);
		log_level.InsertItem("Debug", new Param1<LogLevel>(LogLevel.DEBUG));
		log_level.InsertItem("Info", new Param1<LogLevel>(LogLevel.INFO));
		log_level.InsertItem("Warning", new Param1<LogLevel>(LogLevel.WARNING));
		log_level.InsertItem("Error", new Param1<LogLevel>(LogLevel.ERROR));
		general_group.Insert(log_level);		
		general_group.Insert(new EditBoxNumberPrefab("Auto Save", GetEditor().Settings, "AutoSaveTimer", GetEditor().Settings.AutoSaveTimer.ToString()));
		
		
		GroupPrefab game_group = new GroupPrefab("Game", GetEditor().Settings, string.Empty);
		game_group.Insert(new SliderPrefab("View Distance", GetEditor().Settings, "ViewDistance", GetEditor().Settings.ViewDistance, 0, 20000));
		game_group.Insert(new SliderPrefab("Object View Distance", GetEditor().Settings, "ObjectViewDistance", GetEditor().Settings.ObjectViewDistance, 0, 8000));
		
		GroupPrefab debug_group = new GroupPrefab("Editor", GetEditor().Settings, string.Empty);
		debug_group.Insert(new EditBoxPrefab("Loot Spawns File", GetEditor().Settings, "EditorProtoFile", GetEditor().Settings.EditorProtoFile));
		debug_group.Insert(new EditBoxPrefab("Brush File", GetEditor().Settings, "EditorBrushFile", GetEditor().Settings.EditorBrushFile));
		debug_group.Insert(new CheckBoxPrefab("Modal Dialogs", GetEditor().Settings, "LockCameraDuringDialogs", GetEditor().Settings.LockCameraDuringDialogs));
		debug_group.Insert(new CheckBoxPrefab("Show Bounding Boxes", GetEditor().Settings, "ShowBoundingBoxes", GetEditor().Settings.ShowBoundingBoxes));
		debug_group.Insert(new CheckBoxPrefab("Debug Mode", GetEditor().Settings, "DebugMode", GetEditor().Settings.DebugMode));
		
		AddContent(general_group);
		AddContent(game_group);
		AddContent(debug_group);
		AddButton(DialogResult.OK);
		AddButton("Close", DialogResult.Cancel);
	}
}