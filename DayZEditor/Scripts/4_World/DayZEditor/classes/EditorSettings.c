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
	bool DebugMode 					= false;
	
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
			
			case "ViewDistance": {
				GetGame().GetWorld().SetViewDistance(ViewDistance);
				break;
			}
			
			case "ObjectViewDistance": {
				GetGame().GetWorld().SetObjectViewDistance(ObjectViewDistance);
				break;
			}
		}
	}	
}