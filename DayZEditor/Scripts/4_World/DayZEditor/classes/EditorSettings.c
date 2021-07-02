class EditorSettings: EditorProfileSettings
{	
	// General Settings
	float ViewDistance 				= 8000;
	float ObjectViewDistance 		= 1500;
	int AutoSaveTimer 				= 240;

	// Advanced Settings
	bool LockCameraDuringDialogs 	= true;
	bool ShowBoundingBoxes 			= true;
	bool PreloadObjects				= false;
	bool DisableWorldCache			= false;
	bool ShowScreenLogs				= true;
	bool HighPrecisionCollision		= false;
	bool DebugMode 					= false;
	
	// Marker Settings
	bool MarkerTooltips				= true;
	float MarkerViewDistance 		= 1000;
	int MarkerColor					= COLOR_WHITE_A;
	
	// Theme Settings
	int HighlightColor				= COLOR_SALMON;
	int SelectionColor				= COLOR_BLUE;
	
	// Brush Settings
	bool BrushedObjectMarkers 		= false;
	bool BrushedListItems			= false;
	string EditorBrushFile 			= "$profile:/Editor/EditorBrushes.xml";
	
	// Loot Settings
	string EditorProtoFile 			= "$profile:/Editor/MapGroupProto.xml";
	
	LogLevel SelectedLogLevel 		= LogLevel.INFO;

	// Its a pseduo-controller, preferences dialogs!!
	void PropertyChanged(string property_name)
	{
		switch (property_name) {
						
			case "SelectedLogLevel": {
				EditorLog.Warning("Changed log level to %1", typename.EnumToString(LogLevel, SelectedLogLevel));
				GetEditor().GetEditorHud().GetTemplateController().CurrentLogLevel = SelectedLogLevel;
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
				EditorObjectMap editor_objects = GetEditor().GetObjectManager().GetPlacedObjects();
				foreach (int id, EditorObject editor_object: editor_objects) {
					EditorObjectMarker marker = editor_object.GetMarker();
					if (marker) {
						marker.Update();
					}
				}
				
				break;
			}
		}
	}
	
	void SetDefaults()
	{
		ViewDistance 				= 8000;
		ObjectViewDistance 			= 1500;
		MarkerViewDistance 			= 1000;
		
		AutoSaveTimer 				= 240;

		LockCameraDuringDialogs 	= true;
		ShowBoundingBoxes 			= true;
		PreloadObjects				= false;
		DisableWorldCache			= false;
		ShowScreenLogs				= true;
		DebugMode 					= false;
		
		MarkerColor					= COLOR_WHITE_A;
		HighlightColor				= COLOR_SALMON;
		SelectionColor				= COLOR_BLUE;
	}
}