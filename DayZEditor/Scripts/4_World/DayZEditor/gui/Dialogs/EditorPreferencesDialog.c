class EditorPreferencesDialog: EditorDialogBase
{	
	void EditorPreferencesDialog(string title)
	{
		GroupPrefab general_group = new GroupPrefab("General", m_Editor.Settings, string.Empty);
		
		DropdownListPrefab<LogLevel> log_level("Log Level", m_Editor.Settings, "SelectedLogLevel");
		log_level.InsertItem("Trace", LogLevel.TRACE);
		log_level.InsertItem("Debug", LogLevel.DEBUG);
		log_level.InsertItem("Info", LogLevel.INFO);
		log_level.InsertItem("Warning", LogLevel.WARNING);
		log_level.InsertItem("Error", LogLevel.ERROR);
		general_group.Insert(log_level);
		
		general_group.Insert(new EditBoxNumberPrefab("Auto Save", m_Editor.Settings, "AutoSaveTimer", m_Editor.Settings.AutoSaveTimer.ToString()));
		
		GroupPrefab game_group = new GroupPrefab("Game", m_Editor.Settings, string.Empty);
		game_group.Insert(new SliderPrefab("View Distance", m_Editor.Settings, "ViewDistance", m_Editor.Settings.ViewDistance, 0, 20000));
		game_group.Insert(new SliderPrefab("Object View Distance", m_Editor.Settings, "ObjectViewDistance", m_Editor.Settings.ObjectViewDistance, 0, 8000));
		game_group.Insert(new SliderPrefab("Marker View Distance", m_Editor.Settings, "MarkerViewDistance", m_Editor.Settings.MarkerViewDistance, 100, 5000));
		
		GroupPrefab debug_group = new GroupPrefab("Editor", m_Editor.Settings, string.Empty);
		debug_group.Insert(new EditBoxPrefab("Loot Spawns File", m_Editor.Settings, "EditorProtoFile"));
		debug_group.Insert(new EditBoxPrefab("Brush File", m_Editor.Settings, "EditorBrushFile"));
		debug_group.Insert(new CheckBoxPrefab("Modal Dialogs", m_Editor.Settings, "LockCameraDuringDialogs"));
		debug_group.Insert(new CheckBoxPrefab("Show Bounding Boxes", m_Editor.Settings, "ShowBoundingBoxes"));
		debug_group.Insert(new CheckBoxPrefab("Brushed Object Markers", m_Editor.Settings, "BrushedObjectMarkers"));
		debug_group.Insert(new CheckBoxPrefab("Preload Objects", m_Editor.Settings, "PreloadObjects"));
		debug_group.Insert(new CheckBoxPrefab("Debug Mode", m_Editor.Settings, "DebugMode", m_Editor.Settings.DebugMode));
		
		AddContent(general_group);
		AddContent(game_group);
		AddContent(debug_group);
		AddButton(DialogResult.OK);
		AddButton("Close", DialogResult.Cancel);
	}
	
	protected override void DialogExitButtonCallback(DialogExitButton button)
	{
		if (button.ButtonResult == DialogResult.OK) {
			m_Editor.Settings.Save();
		}
		
		super.DialogExitButtonCallback(button);
	}
}