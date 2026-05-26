[RegisterProfileSetting(EditorSettings, "EDITOR")]
class EditorSettings: ProfileSettings
{	
	bool AutoSaveEnabled			= true;
	float AutoSaveTimer 			= 240;
	
	[RegisterProfileSettingMultistate("ADVANCED", "ConsoleMode", "Console Mode", {"DISABLED", "ENABLED"}, true)]
	int ConsoleMode = 0;
	
	[RegisterProfileSettingMultistate("ADVANCED", "ClearSelectionOnHudHide", "Clear Selection When Hiding UI", {"DISABLED", "ENABLED"})]
	int ClearSelectionOnHudHide = 1;
	
	[RegisterProfileSettingMultistate("ADVANCED", "HideCursorOnPlayerControl", "Hide Cursor When Controlling Player", {"NO", "YES"})]
	int HideCursorOnPlayerControl;

	[RegisterProfileSettingMultistate("ADVANCED", "DrawCameraLightShadows", "Draw Camera Light Shadows", {"NO", "YES"})]
	int DrawCameraLightShadows = 0;

	[RegisterProfileSettingMultistate("PLACEMENT", "QuickMoveMode", "Quick Move Mode", {"WORLD", "CAMERA FLAT", "CAMERA 3D"})]
	int QuickMoveMode = 1;
		
	[RegisterProfileSettingSlider("PLACEMENT", "QuickMoveRate", "Quick Move Speed (m/s)", 0.1, 15)]
	float QuickMoveRate = 2.5;
		
	[RegisterProfileSettingMultistate("PLACEMENT", "SpawnItemsWithAttachments", "Item Spawns", {"EMPTY", "WITH ATTACHMENT"})]
	int SpawnItemsWithAttachments	= 0;
	
//#ifndef COMPONENT_SYSTEM	
	//[RegisterProfileSettingMultistate("PLACEMENT", "AltMoveMode", "Vertical Placement Mode", {"WORLD", "LOCAL"})]
//#endif
	int AltMoveMode = 1;
	
	[RegisterProfileSettingMultistate("ADVANCED", "UseExperimentalLights", "Experimental Lights", {"DISABLED", "ENABLED"})]
	int UseExperimentalLights = 0;
	
	[RegisterProfileSettingMultistate("ADVANCED", "BrushedObjectsRespectOtherObjects", "Brush On Objects", {"DISABLED", "ENABLED"})]
	int BrushedObjectsRespectOtherObjects = 0;
		
	// Set once on launch
	bool HasSelectedConsoleMode = 0;
	
	// todo: stub
	////[RegisterProfileSettingMultistate("PLACEMENT", "AllowPlacementOfPersistentItems", "Place Persistent Items", {"NO", "YES"})]
	bool AllowPlacementOfPersistentItems = false; 
	
	bool ShowScopeZeroObjects		= false;
	bool ShowFavoriteObjects		= false;
	bool AssetsBrowserFavoritesOnly		= false;
	int AssetsBrowserPlatformFilter		= 0;
	int AssetsBrowserPlacementTypeFilter = 0;
	string AssetsBrowserLastTabId		= "structures";
	string AssetsBrowserLastSubtabId	= "buildings";
	ref array<string> AssetsBrowserSelectedSources = {};

	// Camera Settings
	float CameraSpeed				= 25;
	float CameraTilt				= 0;
	
#ifndef COMPONENT_SYSTEM
	[RegisterProfileSettingMultistate("ADVANCED", "RuleOfThirds", "Draw Rule of Thirds", {"NO", "YES"})]
#endif
	bool RuleOfThirds				= false;
	bool AllowBelowGround 			= false;
	
#ifndef COMPONENT_SYSTEM	
	[RegisterProfileSettingMultistate("IMPORT/EXPORT", "CreateSaveBackups", "Create Backup Saves", {"NO", "EVERY 5MIN", "EVERY 15MIN", "EVERY HOUR"})]
#endif
	int CreateSaveBackups = 0;

	// Advanced Settings
#ifndef COMPONENT_SYSTEM	
	[RegisterProfileSettingMultistate("ADVANCED", "CreateCharacterObject", "Create Character Object", {"NO", "YES"})]
#endif
	bool CreateCharacterObject		= false;
	
#ifndef COMPONENT_SYSTEM	
	[RegisterProfileSettingMultistate("ADVANCED", "LockCameraDuringDialogs", "Modal Dialogs", {"NO", "YES"})]
#endif
	bool LockCameraDuringDialogs 	= true;
		
#ifndef COMPONENT_SYSTEM	
	[RegisterProfileSettingMultistate("APPEARANCE", "BoundingBoxSize", "Bounding Box Size", {"OFF", "THIN", "MEDIUM", "THICK", "EXTRA THICCC"})]
#endif
	int BoundingBoxSize = 2;
	
	//[RegisterProfileSettingMultistate("APPEARANCE", "ListItemSize", "List Item Size", {"SMALL", "MEDIUM", "LARGE"})]
	int ListItemSize = 1; // todo: stub
	
	bool PreloadObjects				= false;	
	bool ShowScreenLogs				= true;
	bool HighPrecisionCollision		= false;
	bool MinifyCopyData				= false;
	bool DebugMode 					= false;
	
	// Marker Settings
	bool MarkerTooltips				= true;
	bool MarkerGroundOcclusion		= false;
	
#ifndef COMPONENT_SYSTEM	
	[RegisterProfileSettingSlider("APPEARANCE", "MarkerViewDistance", "Marker Range", 10, 3000)]
#endif
	float MarkerViewDistance 		= 1000.0;
	
#ifndef COMPONENT_SYSTEM	
	[RegisterProfileSettingColor("APPEARANCE", "MarkerColor", "Marker Color", false)]
#endif
	int MarkerColor			= 0xFFF5FFFA;
	
	// Theme Settings
#ifndef COMPONENT_SYSTEM	
	[RegisterProfileSettingMultistate("APPEARANCE", "MarkerSize", "Marker Size", {"SMALL", "MEDIUM", "LARGE", "GIGANTIC"})]
#endif
	int MarkerSize = 1;
	
#ifndef COMPONENT_SYSTEM	
	[RegisterProfileSettingColor("APPEARANCE", "HighlightColor", "Highlight Color", false)]
#endif
	int HighlightColor				= 0xff3d4045;
	
#ifndef COMPONENT_SYSTEM	
	[RegisterProfileSettingColor("APPEARANCE", "SelectionColor", "Selection Color", false)]
#endif
	int SelectionColor				= 0xff6ba5ff;
	
	// Brush Settings
#ifndef COMPONENT_SYSTEM	
	[RegisterProfileSettingMultistate("BRUSHES", "BrushedObjectMarkers", "Brush Object Markers", {"NO", "YES"})]
#endif
	bool BrushedObjectMarkers 		= false;
	
#ifndef COMPONENT_SYSTEM	
	[RegisterProfileSettingMultistate("BRUSHES", "BrushedListItems", "Brush List Items", {"NO", "YES"})]
#endif
	bool BrushedListItems			= false;
	
#ifndef COMPONENT_SYSTEM	
	[RegisterProfileSettingText("PLACEMENT", "BrushFile", "Brush File")]
#endif
	string BrushFile 			= SystemPath.Combine(SystemPath.Combine(SystemPath.Saves(), "Editor"), "Brushes.xml");
	
#ifndef COMPONENT_SYSTEM	
	[RegisterProfileSettingText("PLACEMENT", "ProtoFile", "CE Proto File")]
#endif
	string ProtoFile 			= SystemPath.Combine(SystemPath.Combine(SystemPath.Saves(), "Editor"), "MapGroupProto.xml");
	
	#ifndef COMPONENT_SYSTEM	
	[RegisterProfileSettingText("IMPORT/EXPORT", "BackupDirectory", "Brush File")]
#endif
	string BackupDirectory = SystemPath.Combine(SystemPath.Combine(SystemPath.Saves(), "Editor"), "Backups");
	
#ifndef COMPONENT_SYSTEM	
	[RegisterProfileSettingMultistate("ADVANCED", "SelectedLogLevel", "Log Level", {"TRACE", "DEBUG", "INFO", "WARNING", "ERROR"})]
#endif
	int SelectedLogLevel 		= LogLevel.WARNING;
	
	ref array<string> FavoriteItems = {};
	
	int VersionRequestedNotToSeeDonationDialog = 0;
	int TimesOpened;
	
	float LeftBarPlacement = 380, RightBarPlacement = 380;
	
	void SetDefaults()
	{
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
		
		HighlightColor				= COLOR_SALMON;
		SelectionColor				= COLOR_BLUE;
	}
}