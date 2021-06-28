class EditorSettings: EditorProfileSettings
{	
	float ViewDistance 				= 8000;
	float ObjectViewDistance 		= 1500;
	float MarkerViewDistance 		= 1000;
		
	int AutoSaveTimer 				= 240;

	bool LockCameraDuringDialogs 	= true;
	bool ShowBoundingBoxes 			= true;
	bool PreloadObjects				= false;
	bool DisableWorldCache			= false;
	bool ShowScreenLogs				= true;
	bool DebugMode 					= false;
	
	int MarkerColor					= COLOR_WHITE_A;
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
}