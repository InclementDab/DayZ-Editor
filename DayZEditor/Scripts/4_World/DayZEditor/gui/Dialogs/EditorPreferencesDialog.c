class EditorPreferencesDialog: EditorDialogCategoryBase
{	
	// Used elsewhere to load categories
	static const ref TStringArray CATEGORIES = {
		"#STR_EDITOR_GENERAL",
		"#STR_EDITOR_CAMERA",
		"#STR_EDITOR_MARKERS",
		"#STR_EDITOR_BRUSHES",
		"#STR_EDITOR_THEMES",
		"#STR_EDITOR_ADVANCED",
	};
	
	void EditorPreferencesDialog(string title, string default_group = "General")
	{
		GroupPrefab general_group = new GroupPrefab("General", m_Editor.Settings, string.Empty);

		DropdownListPrefab<LogLevel> log_level("Log Level", m_Editor.Settings, "SelectedLogLevel");
		log_level["Trace"] = LogLevel.TRACE;
		log_level["Debug"] = LogLevel.DEBUG;
		log_level["Info"] = LogLevel.INFO;
		log_level["Warning"] = LogLevel.WARNING;
		log_level["Error"] = LogLevel.ERROR;
		
		general_group.Insert(log_level);
		general_group.Insert(new EditBoxNumberPrefab("Auto Save", m_Editor.Settings, "AutoSaveTimer"));
		general_group.Insert(new SliderPrefab("View Distance", m_Editor.Settings, "ViewDistance", 0, 20000));
		general_group.Insert(new SliderPrefab("Object View Distance", m_Editor.Settings, "ObjectViewDistance", 0, 8000));
		general_group.Insert(new EditBoxNumberPrefab("Quick Move Step", m_Editor.Settings, "QuickMoveStepSize", 0.01));	
		
		general_group.Insert(new CheckBoxPrefab("Show Screen Logs", m_Editor.Settings, "ShowScreenLogs"));
		general_group.Insert(new CheckBoxPrefab("Show Bounding Boxes", m_Editor.Settings, "ShowBoundingBoxes"));
		
		GroupPrefab camera_group = new GroupPrefab("Camera Settings", m_Editor.Settings, string.Empty);
		camera_group.Insert(new SliderPrefab("Camera Speed", m_Editor.Settings, "CameraSpeed", 0, 500));
		camera_group.Insert(new CheckBoxPrefab("Show Rule of 3rds", m_Editor.Settings, "RuleOfThirds"));
		
		GroupPrefab marker_group = new GroupPrefab("Marker Settings", m_Editor.Settings, string.Empty);
		marker_group.Insert(new CheckBoxPrefab("Show Tooltips", m_Editor.Settings, "MarkerTooltips"));
		marker_group.Insert(new SliderPrefab("View Distance", m_Editor.Settings, "MarkerViewDistance", 100, 5000));
		marker_group.Insert(new ColorPickerPrefab("Marker Color", m_Editor.Settings, "MarkerPrimaryColor"));
		
		GroupPrefab brush_settings = new GroupPrefab("Brush Settings", m_Editor.Settings, string.Empty);
		brush_settings.Insert(new EditBoxPrefab("Brush File", m_Editor.Settings, "EditorBrushFile"));
		brush_settings.Insert(new CheckBoxPrefab("Show Object Markers", m_Editor.Settings, "BrushedObjectMarkers"));
		brush_settings.Insert(new CheckBoxPrefab("Show List Items", m_Editor.Settings, "BrushedListItems"));
		
		GroupPrefab advanced_group = new GroupPrefab("Advanced Settings", m_Editor.Settings, string.Empty);
		advanced_group.Insert(new EditBoxPrefab("Loot Spawns File", m_Editor.Settings, "EditorProtoFile"));
		advanced_group.Insert(new CheckBoxPrefab("Modal Dialogs", m_Editor.Settings, "LockCameraDuringDialogs"));
		advanced_group.Insert(new CheckBoxPrefab("Preload Objects", m_Editor.Settings, "PreloadObjects"));
		advanced_group.Insert(new CheckBoxPrefab("High Precision Collision", m_Editor.Settings, "HighPrecisionCollision"));
		advanced_group.Insert(new CheckBoxPrefab("Export Backup on Bin", m_Editor.Settings, "ExportBackupFile"));
		advanced_group.Insert(new CheckBoxPrefab("Debug Mode", m_Editor.Settings, "DebugMode"));
		
		GroupPrefab colors_group = new GroupPrefab("Colors", m_Editor.Settings, string.Empty);
		colors_group.Insert(new ColorPickerPrefab("Highlight Color", m_Editor.Settings, "HighlightColor"));
		colors_group.Insert(new ColorPickerPrefab("Selected Color", m_Editor.Settings, "SelectionColor"));
		
		DialogCategoryListItem general_category("#STR_EDITOR_GENERAL");
		general_category.AddContent(general_group);
		AddContent(general_category);
		
		DialogCategoryListItem camera_category("#STR_EDITOR_CAMERA");
		camera_category.AddContent(camera_group);
		AddContent(camera_category);
		
		DialogCategoryListItem marker_category("#STR_EDITOR_MARKERS");
		marker_category.AddContent(marker_group);
		AddContent(marker_category);
		
		DialogCategoryListItem brush_category("#STR_EDITOR_BRUSHES");
		brush_category.AddContent(brush_settings);
		AddContent(brush_category);
		
		DialogCategoryListItem theme_category("#STR_EDITOR_THEMES");
		theme_category.AddContent(colors_group);
		AddContent(theme_category);
		
		DialogCategoryListItem advanced_category("#STR_EDITOR_ADVANCED");
		advanced_category.AddContent(advanced_group);
		AddContent(advanced_category);
		
		// Assign Default Category
		for (int i = 0; i < m_DialogCategoryBaseController.DialogCategoryData.Count(); i++) {
			DialogCategoryListItem list_item = m_DialogCategoryBaseController.DialogCategoryData[i];
			if (list_item.GetTemplateController().Caption == default_group) {
				SetActiveCategory(list_item);
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
	
	override string GetIcon() 
	{
		return "set:dayz_gui image:settings";
	}
	
	void SetDefaults()
	{
		EditorLog.Trace("EditorPreferencesDialog::SetDefaults");
		m_Editor.Settings.SetDefaults();
		
		for (int i = 0; i < m_DialogBaseController.DialogContentData.Count(); i++) {
			GroupPrefab group = GroupPrefab.Cast(m_DialogBaseController.DialogContentData[i]);
			if (!group) {
				continue;		
			}
			
			array<ref ScriptView> children = group.GetChildren();
			for (int j = 0; j < children.Count(); j++) {
				if (!children[j]) {
					continue;
				}
				
				g_Script.Call(children[j], "ResetToDefault", null);
			}		
		}
	}
}