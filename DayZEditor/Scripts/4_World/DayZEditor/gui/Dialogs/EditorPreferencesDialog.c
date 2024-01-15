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
	
			// autoptr since assigned prior to the handover to the array
	protected autoptr CheckBoxPrefab m_AutoSaveToggle;
	protected autoptr ScriptView m_AutoSaveTimer;
	
	void EditorPreferencesDialog(string title, string default_group = "General")
	{
		GroupPrefab general_group = new GroupPrefab("#STR_EDITOR_GENERAL", m_Editor.GeneralSettings, string.Empty);

		DropdownListPrefab<LogLevel> log_level("#STR_EDITOR_LOG_LEVEL", m_Editor.GeneralSettings, "SelectedLogLevel");
		log_level["Trace"] = LogLevel.TRACE;
		log_level["Debug"] = LogLevel.DEBUG;
		log_level["Info"] = LogLevel.INFO;
		log_level["Warning"] = LogLevel.WARNING;
		log_level["Error"] = LogLevel.ERROR;
		
		general_group.Insert(log_level);
		
		m_AutoSaveToggle = new CheckBoxPrefab("", m_Editor.GeneralSettings, "AutoSaveEnabled");
		general_group.Insert(m_AutoSaveToggle);
		m_AutoSaveTimer = new SliderPrefab("#STR_EDITOR_AUTO_SAVE_TIMER", m_Editor.GeneralSettings, "AutoSaveTimer", 10, 600);
		general_group.Insert(m_AutoSaveTimer);
		
		// update view state of auto save timer
		PrefabBaseController<bool> controller = m_AutoSaveToggle.GetPrefabController();
		m_AutoSaveTimer.GetLayoutRoot().Show(controller.Value);
		
		
		
		GroupPrefab camera_group = new GroupPrefab("#STR_EDITOR_CAMERA", m_Editor.GeneralSettings, string.Empty);
		camera_group.Insert(new SliderPrefab("#STR_EDITOR_CAMERA_SPEED", m_Editor.GeneralSettings, "CameraSpeed", 0, 500));
		camera_group.Insert(new SliderPrefab("#STR_EDITOR_CAMERA_TILT", m_Editor.GeneralSettings, "CameraTilt", -90, 90));
		camera_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW_RULE_OF_THIRDS", m_Editor.GeneralSettings, "RuleOfThirds"));
		camera_group.Insert(new CheckBoxPrefab("Allow Below Ground", m_Editor.GeneralSettings, "AllowBelowGround"));
		
		GroupPrefab marker_group = new GroupPrefab("#STR_EDITOR_MARKERS", m_Editor.GeneralSettings, string.Empty);
		marker_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW_TOOLTIPS", m_Editor.GeneralSettings, "MarkerTooltips"));
		marker_group.Insert(new CheckBoxPrefab("#STR_EDITOR_GROUND_OCCLUSION", m_Editor.GeneralSettings, "MarkerGroundOcclusion"));
		marker_group.Insert(new SliderPrefab("#STR_EDITOR_VIEW_DISTANCE", m_Editor.GeneralSettings, "MarkerViewDistance", 1, 5000));
		marker_group.Insert(new ColorPickerPrefab("#STR_EDITOR_MARKER_COLOR", m_Editor.GeneralSettings, "MarkerPrimaryColor"));
		
		GroupPrefab brush_settings = new GroupPrefab("#STR_EDITOR_BRUSHES", m_Editor.GeneralSettings, string.Empty);
		brush_settings.Insert(new EditBoxPrefab("#STR_EDITOR_BRUSH_FILE", m_Editor.GeneralSettings, "EditorBrushFile"));
		brush_settings.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW_OBJECT_MARKERS", m_Editor.GeneralSettings, "BrushedObjectMarkers"));
		brush_settings.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW_LIST_ITEMS", m_Editor.GeneralSettings, "BrushedListItems"));
		
		GroupPrefab advanced_group = new GroupPrefab("#STR_EDITOR_ADVANCED", m_Editor.GeneralSettings, string.Empty);
		advanced_group.Insert(new EditBoxPrefab("#STR_EDITOR_LOOT_SPAWN_FILE", m_Editor.GeneralSettings, "EditorProtoFile"));
		advanced_group.Insert(new CheckBoxPrefab("#STR_EDITOR_MODAL_DIALOGS", m_Editor.GeneralSettings, "LockCameraDuringDialogs"));
		advanced_group.Insert(new CheckBoxPrefab("#STR_EDITOR_PRELOAD_OBJECTS", m_Editor.GeneralSettings, "PreloadObjects"));
		advanced_group.Insert(new CheckBoxPrefab("Show Objects with Scope 0", m_Editor.GeneralSettings, "ShowScopeZeroObjects"));
		advanced_group.Insert(new CheckBoxPrefab("#STR_EDITOR_HIGH_PRECISION_COLLISION", m_Editor.GeneralSettings, "HighPrecisionCollision"));
		advanced_group.Insert(new CheckBoxPrefab("#STR_EDITOR_DEBUG_MODE", m_Editor.GeneralSettings, "DebugMode"));
		
		GroupPrefab colors_group = new GroupPrefab("#STR_EDITOR_THEMES", m_Editor.GeneralSettings, string.Empty);
		colors_group.Insert(new ColorPickerPrefab("#STR_EDITOR_HIGHLIGHT_COLOR", m_Editor.GeneralSettings, "HighlightColor"));
		colors_group.Insert(new ColorPickerPrefab("#STR_EDITOR_SELECTED_COLOR", m_Editor.GeneralSettings, "SelectionColor"));
		
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
		AddButton("#STR_EDITOR_CLOSE", DialogResult.Cancel);
		AddButton("#STR_EDITOR_DEFAULTS", "SetDefaults");
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		// update view state of autosave timer
		PrefabBaseController<bool> controller = m_AutoSaveToggle.GetPrefabController();
		m_AutoSaveTimer.GetLayoutRoot().Show(controller.Value);
		
		return super.OnClick(w, x, y, button);
	}
	
	protected override void DialogExitButtonCallback(DialogExitButton button)
	{
		if (button.ButtonResult == DialogResult.OK) {
			m_Editor.GeneralSettings.Save();
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
		m_Editor.GeneralSettings.SetDefaults();
		
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