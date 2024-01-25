[RegisterProfileSetting(EditorProfileSettings, "EDITOR")]
class EditorProfileSettings: ProfileSettings
{	
	
	/*
	
	general_group.Insert(new SliderPrefab("", m_Editor.Settings, "ViewDistance", 1, 20000));
	general_group.Insert(new SliderPrefab("#", m_Editor.Settings, "ObjectViewDistance", 1, 8000));
	general_group.Insert(new EditBoxNumberPrefab("#STR_EDITOR_QUICK_MOVE_STEP", m_Editor.Settings, "QuickMoveStepSize", 0.01));	
	general_group.Insert(new CheckBoxPrefab("#STR_EDITOR_QUICK_MOVE_FOLLOWS", m_Editor.Settings, "QuickMoveFollowsCamera"));	
	general_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SPAWN_ITEMS_WITH_ATTACHMENTS", m_Editor.Settings, "SpawnItemsWithAttachments"));	
	
	general_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW_SCREEN_LOGS", m_Editor.Settings, "ShowScreenLogs"));
	general_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW_BOUNDING_BOXES", m_Editor.Settings, "ShowBoundingBoxes"));
	
	*/
	
	// General Settings
	[RegisterProfileSettingSlider("#STR_EDITOR_GENERAL", "ViewDistance", "#STR_EDITOR_VIEW_DISTANCE", 200, 40000)]
	float ViewDistance 				= 3000;
	
	[RegisterProfileSettingSlider("#STR_EDITOR_GENERAL", "ObjectViewDistance", "#STR_EDITOR_OBJECT_VIEW_DISTANCE", 200, 40000)]
	float ObjectViewDistance 		= 2400;
	
	[RegisterProfileSettingMultistate("#STR_EDITOR_GENERAL", "AutoSaveEnabled", "#STR_EDITOR_AUTO_SAVE", {"#menu_yes", "#menu_no"})]
	int AutoSaveEnabled			= true; // << remove!!!
	
	[RegisterProfileSettingSlider("STR_EDITOR_CAMERA", "Smoothing", "Smoothing", 0.0, 1.0)]
	float Smoothing = 0.0;
	
	//float AutoSaveTimer 			= 240;
	float QuickMoveStepSize			= 0.1;
	bool QuickMoveFollowsCamera		= false;
	bool SpawnItemsWithAttachments	= true;
	bool ShowScopeZeroObjects		= false;
	
	// Advanced Settings
	bool LockCameraDuringDialogs 	= true;
	bool ShowBoundingBoxes 			= true;
	bool PreloadObjects				= false;	
	bool ShowScreenLogs				= true;
	bool HighPrecisionCollision		= false;
	bool DebugMode 					= false;
	
	// Marker Settings
	bool MarkerTooltips				= true;
	bool MarkerGroundOcclusion		= false;
	float MarkerViewDistance 		= 1000;
	int MarkerPrimaryColor			= COLOR_WHITE;
	
	// Theme Settings
	[RegisterProfileSettingColor("#STR_EDITOR_THEMES", "HighlightColor", "#STR_EDITOR_HIGHLIGHT_COLOR", false)]
	int HighlightColor				= COLOR_SALMON;
	
	[RegisterProfileSettingColor("#STR_EDITOR_THEMES", "SelectionColor", "#STR_EDITOR_SELECTION_COLOR", false)]
	int SelectionColor				= COLOR_BLUE;
	
	// Brush Settings
	bool BrushedObjectMarkers 		= false;
	bool BrushedListItems			= false;
	//string EditorBrushFile 			= Editor.ROOT_DIRECTORY + "Brushes.xml";
	
	// Loot Settings
	//string EditorProtoFile 			= Editor.ROOT_DIRECTORY + "MapGroupProto.xml";
	
	LogLevel SelectedLogLevel 		= LogLevel.INFO;
	
	override void ApplyOptions()
	{		
		GetGame().GetWorld().SetViewDistance(ViewDistance);
		GetGame().GetWorld().SetObjectViewDistance(ObjectViewDistance);
		

		//GetDayZGame().GetEditor().GetHud().ShowRuleOfThirds(RuleOfThirds);
	}
}