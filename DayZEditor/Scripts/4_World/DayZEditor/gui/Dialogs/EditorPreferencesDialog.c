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
		EditorSettings settings = GetEditor().GetSettings();
		GroupPrefab general_group = new GroupPrefab("#STR_EDITOR_GENERAL", settings, string.Empty);

		DropdownListPrefab<LogLevel> log_level("#STR_EDITOR_LOG_LEVEL", settings, "SelectedLogLevel");
		log_level["Trace"] = LogLevel.TRACE;
		log_level["Debug"] = LogLevel.DEBUG;
		log_level["Info"] = LogLevel.INFO;
		log_level["Warning"] = LogLevel.WARNING;
		log_level["Error"] = LogLevel.ERROR;
		
		general_group.Insert(log_level);
						
		general_group.Insert(new EditBoxNumberPrefab("#STR_EDITOR_QUICK_MOVE_STEP", settings, "QuickMoveStepSize", 0.01));	
		general_group.Insert(new CheckBoxPrefab("#STR_EDITOR_QUICK_MOVE_FOLLOWS", settings, "QuickMoveFollowsCamera"));	
		general_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SPAWN_ITEMS_WITH_ATTACHMENTS", settings, "SpawnItemsWithAttachments"));	
		
		general_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW_SCREEN_LOGS", settings, "ShowScreenLogs"));
		general_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW_BOUNDING_BOXES", settings, "ShowBoundingBoxes"));
		
		
		GroupPrefab camera_group = new GroupPrefab("#STR_EDITOR_CAMERA", settings, string.Empty);
		if (GetEditor().GetCamera().GetSettings().LegacyCamera) {
			camera_group.Insert(new SliderPrefab("#STR_EDITOR_CAMERA_SPEED", settings, "CameraSpeed", 0, 500));
			camera_group.Insert(new SliderPrefab("#STR_EDITOR_CAMERA_TILT", settings, "CameraTilt", -90, 90));
			camera_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW_RULE_OF_THIRDS", settings, "RuleOfThirds"));
			camera_group.Insert(new CheckBoxPrefab("Allow Below Ground", settings, "AllowBelowGround"));
		}
		
		GroupPrefab marker_group = new GroupPrefab("#STR_EDITOR_MARKERS", settings, string.Empty);
		marker_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW_TOOLTIPS", settings, "MarkerTooltips"));
		marker_group.Insert(new CheckBoxPrefab("#STR_EDITOR_GROUND_OCCLUSION", settings, "MarkerGroundOcclusion"));
		marker_group.Insert(new SliderPrefab("#STR_EDITOR_VIEW_DISTANCE", settings, "MarkerViewDistance", 1, 5000));
		marker_group.Insert(new ColorPickerPrefab("#STR_EDITOR_MARKER_COLOR", settings, "MarkerPrimaryColor"));
		
		GroupPrefab brush_settings = new GroupPrefab("#STR_EDITOR_BRUSHES", settings, string.Empty);
		brush_settings.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW_OBJECT_MARKERS", settings, "BrushedObjectMarkers"));
		brush_settings.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW_LIST_ITEMS", settings, "BrushedListItems"));
		
		GroupPrefab advanced_group = new GroupPrefab("#STR_EDITOR_ADVANCED", settings, string.Empty);
		advanced_group.Insert(new EditBoxPrefab("#STR_EDITOR_LOOT_SPAWN_FILE", settings, "EditorProtoFile"));
		advanced_group.Insert(new CheckBoxPrefab("#STR_EDITOR_MODAL_DIALOGS", settings, "LockCameraDuringDialogs"));
		advanced_group.Insert(new CheckBoxPrefab("#STR_EDITOR_PRELOAD_OBJECTS", settings, "PreloadObjects"));
		advanced_group.Insert(new CheckBoxPrefab("#STR_EDITOR_HIGH_PRECISION_COLLISION", settings, "HighPrecisionCollision"));
		advanced_group.Insert(new CheckBoxPrefab("Minify Clipboard Data", settings, "MinifyCopyData"));
		advanced_group.Insert(new CheckBoxPrefab("#STR_EDITOR_DEBUG_MODE", settings, "DebugMode"));
		
		GroupPrefab colors_group = new GroupPrefab("#STR_EDITOR_THEMES", settings, string.Empty);
		colors_group.Insert(new ColorPickerPrefab("#STR_EDITOR_HIGHLIGHT_COLOR", settings, "HighlightColor"));
		colors_group.Insert(new ColorPickerPrefab("#STR_EDITOR_SELECTED_COLOR", settings, "SelectionColor"));
		
		DialogCategoryListItem general_category("#STR_EDITOR_GENERAL");
		general_category.AddContent(general_group);
		AddContent(general_category);
		
		if (GetEditor().GetCamera().GetSettings().LegacyCamera) {
			DialogCategoryListItem camera_category("#STR_EDITOR_CAMERA");
			camera_category.AddContent(camera_group);
			AddContent(camera_category);
		}
		
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
		AddButton("#STR_EDITOR_CLOSE", DialogResult.Cancel);
		AddButton("#STR_EDITOR_DEFAULTS", "SetDefaults");
	}
		
	protected override void DialogExitButtonCallback(DialogExitButton button)
	{
		if (button.ButtonResult == DialogResult.OK) {
			GetEditor().GetSettings().Save();
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
		GetEditor().GetSettings().SetDefaults();
		
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