class EditorPreferencesDialog: EditorDialogCategoryBase
{	
	void EditorPreferencesDialog(string title, string default_group = "General")
	{
		GroupPrefab general_group = new GroupPrefab("General", m_Editor.Settings, string.Empty);

		DropdownListPrefab<LogLevel> log_level("Log Level", m_Editor.Settings, "SelectedLogLevel");
		log_level.InsertItem("Trace", LogLevel.TRACE);
		log_level.InsertItem("Debug", LogLevel.DEBUG);
		log_level.InsertItem("Info", LogLevel.INFO);
		log_level.InsertItem("Warning", LogLevel.WARNING);
		log_level.InsertItem("Error", LogLevel.ERROR);
		
		general_group.Insert(log_level);
		general_group.Insert(new EditBoxNumberPrefab("Auto Save", m_Editor.Settings, "AutoSaveTimer")); //m_Editor.Settings.AutoSaveTimer.ToString()
		general_group.Insert(new SliderPrefab("View Distance", m_Editor.Settings, "ViewDistance", 0, 20000));
		general_group.Insert(new SliderPrefab("Object View Distance", m_Editor.Settings, "ObjectViewDistance", 0, 8000));
		general_group.Insert(new SliderPrefab("Marker View Distance", m_Editor.Settings, "MarkerViewDistance", 100, 5000));
		
		GroupPrefab brush_settings = new GroupPrefab("Brush Settings", m_Editor.Settings, string.Empty);
		brush_settings.Insert(new EditBoxPrefab("Brush File", m_Editor.Settings, "EditorBrushFile"));
		brush_settings.Insert(new CheckBoxPrefab("Show Object Markers", m_Editor.Settings, "BrushedObjectMarkers"));
		brush_settings.Insert(new CheckBoxPrefab("Show List Items", m_Editor.Settings, "BrushedListItems"));
		
		GroupPrefab advanced_group = new GroupPrefab("Advanced Settings", m_Editor.Settings, string.Empty);
		advanced_group.Insert(new EditBoxPrefab("Loot Spawns File", m_Editor.Settings, "EditorProtoFile"));
		advanced_group.Insert(new CheckBoxPrefab("Modal Dialogs", m_Editor.Settings, "LockCameraDuringDialogs"));
		advanced_group.Insert(new CheckBoxPrefab("Show Screen Logs", m_Editor.Settings, "ShowScreenLogs"));
		advanced_group.Insert(new CheckBoxPrefab("Show Bounding Boxes", m_Editor.Settings, "ShowBoundingBoxes"));
		advanced_group.Insert(new CheckBoxPrefab("Preload Objects", m_Editor.Settings, "PreloadObjects"));
		advanced_group.Insert(new CheckBoxPrefab("Disable World Cache", m_Editor.Settings, "DisableWorldCache"));
		advanced_group.Insert(new CheckBoxPrefab("Debug Mode", m_Editor.Settings, "DebugMode"));
		
		GroupPrefab colors_group = new GroupPrefab("Colors", m_Editor.Settings, string.Empty);
		colors_group.Insert(new ColorPickerPrefab("Marker Color", m_Editor.Settings, "MarkerColor"));
		colors_group.Insert(new ColorPickerPrefab("Highlight Color", m_Editor.Settings, "HighlightColor"));
		colors_group.Insert(new ColorPickerPrefab("Selected Color", m_Editor.Settings, "SelectionColor"));
		
		DialogCategoryListItem general_category("General");
		general_category.AddContent(general_group);
		AddContent(general_category);
		
		DialogCategoryListItem brush_category("Brushes");
		brush_category.AddContent(brush_settings);
		AddContent(brush_category);
		
		DialogCategoryListItem theme_category("Themes");
		theme_category.AddContent(colors_group);
		AddContent(theme_category);
		
		DialogCategoryListItem advanced_category("Advanced");
		advanced_category.AddContent(advanced_group);
		AddContent(advanced_category);
		
		// Assign Active Category
		switch (default_group) {
			case "General": {
				SetActiveCategory(general_category);
				break;
			}
			
			case "Brushes": {
				SetActiveCategory(brush_category);
				break;
			}
			
			case "Themes": {
				SetActiveCategory(theme_category);
				break;
			}
			
			case "Advanced": {
				SetActiveCategory(advanced_category);
				break;
			}
		}
		
		AddButton(DialogResult.OK);
		AddButton("Close", DialogResult.Cancel);
		AddButton("Defaults", "SetDefaults");
	}
	
	protected override void DialogExitButtonCallback(DialogExitButton button)
	{
		if (button.ButtonResult == DialogResult.OK) {
			m_Editor.Settings.Save();
		}
		
		super.DialogExitButtonCallback(button);
	}
	
	void SetDefaults()
	{
		EditorLog.Trace("EditorPreferencesDialog::SetDefaults");
		m_Editor.Settings.SetDefaults();
		
		for (int i = 0; i < m_DialogBaseController.DialogContentData.Count(); i++) {
			ScriptView view = m_DialogBaseController.DialogContentData[i];
			if (view.IsInherited(GroupPrefab)) {
				GroupPrefab group = GroupPrefab.Cast(view);
				array<ref ScriptView> children = group.GetChildren();
				for (int j = 0; j < children.Count(); j++) {
					ScriptView child = children[j];
					if (child) {
						g_Script.Call(child, "ResetToDefault", null);
					}
				}				
			}
		}
	}
}