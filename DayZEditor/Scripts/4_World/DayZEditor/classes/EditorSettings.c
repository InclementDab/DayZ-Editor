[RegisterProfileSetting(EditorSettings, "#STR_EDITOR_EDITOR")]
class EditorSettings: ProfileSettings
{	
	bool AutoSaveEnabled			= true;
	float AutoSaveTimer 			= 240;
	
	[RegisterProfileSettingSlider("PLACEMENT", "QuickMoveStepSize", "Quick Move Speed (m/s)", 0.1, 4)]
	float QuickMoveStepSize			= 0.1;
	
	[RegisterProfileSettingMultistate("PLACEMENT", "QuickMoveFollowsCamera", "Quick Move Mode", {"WORLD", "LOCAL"})]
	int QuickMoveFollowsCamera		= 0;
	
	[RegisterProfileSettingMultistate("PLACEMENT", "SpawnItemsWithAttachments", "Item Spawns", {"EMPTY", "WITH ATTACHMENT"})]
	int SpawnItemsWithAttachments	= 0;
	
	[RegisterProfileSettingMultistate("PLACEMENT", "AllowPlacementOfPersistentItems", "Place Persistent Items", {"NO", "YES"})]
	bool AllowPlacementOfPersistentItems = false; 
	
	bool ShowScopeZeroObjects		= false;
	bool ShowFavoriteObjects		= false;

	// Camera Settings
	float CameraSpeed				= 25;
	float CameraTilt				= 0;
	bool RuleOfThirds				= false;
	bool AllowBelowGround 			= false;
	
	[RegisterProfileSettingMultistate("EXPORT/IMPORT", "CreateSaveBackups", "Create Backup Saves", {"NO", "EVERY 5MIN", "EVERY 15MIN", "EVERY HOUR"})]
	int CreateSaveBackups = 0;

	// Advanced Settings
	[RegisterProfileSettingMultistate("ADVANCED", "CreateCharacterObject", "Create Character Object", {"NO", "YES"})]
	bool CreateCharacterObject		= false;
	
	[RegisterProfileSettingMultistate("ADVANCED", "LockCameraDuringDialogs", "Modal Dialogs", {"NO", "YES"})]
	bool LockCameraDuringDialogs 	= true;
		
	[RegisterProfileSettingMultistate("APPEARANCE", "BoundingBoxSize", "Bounding Box Size", {"OFF", "THIN", "MEDIUM", "THICK", "EXTRA THICC"})]
	int BoundingBoxSize = 2;
	
	bool PreloadObjects				= false;	
	bool ShowScreenLogs				= true;
	bool HighPrecisionCollision		= false;
	bool MinifyCopyData				= false;
	bool DebugMode 					= false;
	
	// Marker Settings
	bool MarkerTooltips				= true;
	bool MarkerGroundOcclusion		= false;
	
	[RegisterProfileSettingSlider("APPEARANCE", "MarkerViewDistance", "Marker Range", 10, 3000)]
	float MarkerViewDistance 		= 1000.0;
	
	int MarkerPrimaryColor			= COLOR_WHITE;
	
	// Theme Settings
	[RegisterProfileSettingMultistate("APPEARANCE", "MarkerSize", "Marker Size", {"SMALL", "MEDIUM", "LARGE", "GIGANTIC"})]
	int MarkerSize = 1;
	
	[RegisterProfileSettingColor("APPEARANCE", "HighlightColor", "Highlight Color", false)]
	int HighlightColor				= 0xff3d4045;
	
	[RegisterProfileSettingColor("APPEARANCE", "SelectionColor", "Selection Color", false)]
	int SelectionColor				= 0xff6ba5ff;
	
	// Brush Settings
	[RegisterProfileSettingMultistate("PLACEMENT", "BrushedObjectMarkers", "Brush Object Markers", {"NO", "YES"})]
	bool BrushedObjectMarkers 		= false;
	
	[RegisterProfileSettingMultistate("PLACEMENT", "BrushedListItems", "Brush List Items", {"NO", "YES"})]
	bool BrushedListItems			= false;
	
	[RegisterProfileSettingText("PLACEMENT", "EditorBrushFile", "Brush File")]
	string EditorBrushFile 			= SystemPath.Combine(Editor.ROOT_DIRECTORY, "Brushes.xml");
	
	[RegisterProfileSettingText("PLACEMENT", "EditorBrushFile", "CE Proto File")]
	string EditorProtoFile 			= SystemPath.Combine(Editor.ROOT_DIRECTORY, "MapGroupProto.xml");
	
	[RegisterProfileSettingMultistate("ADVANCED", "SelectedLogLevel", "Log Level", {"TRACE", "DEBUG", "INFO", "WARNING", "ERROR"})]
	int SelectedLogLevel 		= LogLevel.WARNING;
	
	ref array<string> FavoriteItems = {};
	
	int VersionRequestedNotToSeeDonationDialog = 0;
	int TimesOpened;
	
	float LeftBarPlacement = 380, RightBarPlacement = 380;

	// Its a pseduo-controller, preferences dialogs!!
	void PropertyChanged(string property_name)
	{		
		switch (property_name) {
						
			case "SelectedLogLevel": {
				EditorLog.Warning("Changed log level to %1", typename.EnumToString(LogLevel, SelectedLogLevel));
				break; 
			}
						
			case "ShowScreenLogs": {
				GetEditor().GetEditorHud().ShowScreenLogs(ShowScreenLogs);
				break;
			}
			
			case "MarkerColor": {
				EditorObjectMap editor_objects = GetEditor().GetObjectManager().GetPlacedObjects();
				foreach (int id, EditorObject editor_object: editor_objects) {
					EditorObjectMarker marker = editor_object.GetMarker();
					if (marker) {
						//marker.Update();
					}
				}
				
				break;
			}
						
			case "RuleOfThirds": {
				GetEditor().GetEditorHud().ShowRuleOfThirds(RuleOfThirds);
				break;
			}
		}
	}
	
	void SetDefaults()
	{
		QuickMoveStepSize			= 0.1; // dont know what this does (not changed)
		CameraSpeed					= 0.05; // 25
		CameraTilt					= 0.5; // 0
		MarkerViewDistance 			= 0.2; // 1000
		
		AutoSaveEnabled				= true;
		AutoSaveTimer 				= 240;

		LockCameraDuringDialogs 	= true;
		PreloadObjects				= false;
		ShowScreenLogs				= true;
		DebugMode 					= false;
		MinifyCopyData = false;
		
		MarkerPrimaryColor			= COLOR_WHITE_A;
		HighlightColor				= COLOR_SALMON;
		SelectionColor				= COLOR_BLUE;
	}
}