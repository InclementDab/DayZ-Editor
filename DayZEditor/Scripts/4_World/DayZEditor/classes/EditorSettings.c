[RegisterProfileSetting(EditorGeneralSettings, "#STR_EDITOR_GENERAL")]
class EditorGeneralSettings: ProfileSettings
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
	bool AutoSaveEnabled			= true; // << remove!!!
	
	
	//float AutoSaveTimer 			= 240;
	float QuickMoveStepSize			= 0.1;
	bool QuickMoveFollowsCamera		= false;
	bool SpawnItemsWithAttachments	= true;
	bool ShowScopeZeroObjects		= false;

	// Camera Settings
	float CameraSpeed				= 25;
	float CameraTilt				= 0;
	bool RuleOfThirds				= false;
	bool AllowBelowGround 			= false;
	
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
	int SelectionColor				= COLOR_BLUE;
	
	// Brush Settings
	bool BrushedObjectMarkers 		= false;
	bool BrushedListItems			= false;
	string EditorBrushFile 			= Editor.ROOT_DIRECTORY + "Brushes.xml";
	
	// Loot Settings
	string EditorProtoFile 			= Editor.ROOT_DIRECTORY + "MapGroupProto.xml";
	
	LogLevel SelectedLogLevel 		= LogLevel.INFO;

	
	
	void SetDefaults()
	{
		QuickMoveStepSize			= 0.1; // dont know what this does (not changed)
		CameraSpeed					= 0.05; // 25
		CameraTilt					= 0.5; // 0
		ViewDistance 				= 0.065; // 3000 .15
		ObjectViewDistance 			= 0.135; // 3000
		MarkerViewDistance 			= 0.2; // 1000
		
		LockCameraDuringDialogs 	= true;
		ShowBoundingBoxes 			= true;
		PreloadObjects				= false;
		ShowScreenLogs				= true;
		DebugMode 					= false;
		
		MarkerPrimaryColor			= COLOR_WHITE_A;
		HighlightColor				= COLOR_SALMON;
		SelectionColor				= COLOR_BLUE;
	}	
}


/*
// Its a pseduo-controller, preferences dialogs!!
	void PropertyChanged(string property_name)
	{		
		switch (property_name) {
						
			case "SelectedLogLevel": {
				EditorLog.Warning("Changed log level to %1", typename.EnumToString(LogLevel, SelectedLogLevel));
				break;
			}
			
			case "ViewDistance":
			case "ObjectViewDistance": {
				// This might break, not sure
				GetGame().GetWorld().SetPreferredViewDistance(ViewDistance);
				
				GetGame().GetWorld().SetViewDistance(ViewDistance);
				GetGame().GetWorld().SetObjectViewDistance(ObjectViewDistance);
				break;
			}
			
			case "ShowScreenLogs": {
				GetEditor().GetEditorHud().ShowScreenLogs(ShowScreenLogs);
				break;
			}
			
			case "MarkerColor": {
				array<ref EditorObject> editor_objects = GetEditor().GetPlacedObjects();
				foreach (EditorObject editor_object: editor_objects) {
					EditorObjectMarker marker = editor_object.GetMarker();
					if (marker) {
						//marker.Update();
					}
				}
				
				break;
			}
			
			case "CameraSpeed": {
				GetEditor().GetCamera().Speed = CameraSpeed;
				break;
			}
			
			case "RuleOfThirds": {
				GetEditor().GetEditorHud().ShowRuleOfThirds(RuleOfThirds);
				break;
			}
		}
	}