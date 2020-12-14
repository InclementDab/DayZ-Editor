/*static EditorSettings LoadSettings(string filename)
{
	EditorLog.Trace("EditorSettings::Load");
	
	EditorSettings settings = new EditorSettings();
	
	// Generate Initial File
	if (!FileExist(filename)) {
		SaveSettings(settings, filename);
		return settings;
	}
	
	EditorLog.Info("Loading EditorSettings from %1", filename);
	// Why the fuck doesnt this load when i RELOAD the editor?!?!?!??!?!?!?!?!!
	// B R U H
	JsonFileLoader<EditorSettings>.JsonLoadFile(filename, settings);
	return settings;
}


static void SaveSettings(EditorSettings settings, string filename)
{
	EditorLog.Trace("EditorSettings::Save");
	
	EditorLog.Info("Saving EditorSettings to %1", filename);
	JsonFileLoader<EditorSettings>.JsonSaveFile(filename, settings);
}
	*/
class EditorSettings: Controller
{	
	float ViewDistance = 8000;
	float ObjectViewDistance = 1500;
	float MarkerViewDistance = 1000;
		
	int AutoSaveTimer = 240;

	bool LockCameraDuringDialogs = true;
	bool ShowBoundingBoxes = true;
	bool BrushedObjectMarkers;
	bool DebugMode;
	
	string EditorProtoFile = "$profile:/Editor/MapGroupProto.xml";
	string EditorBrushFile = "$profile:/Editor/EditorBrushes.xml";
	
	[NonSerialized()]
	ref DropdownListPrefabItem SelectedLogLevel;
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
						
			case "SelectedLogLevel": {
				
				if (SelectedLogLevel && SelectedLogLevel.GetTemplateController() && SelectedLogLevel.GetTemplateController().UserData) {
					Param1<LogLevel> p = Param1<LogLevel>.Cast(SelectedLogLevel.GetTemplateController().UserData);
					if (p) {
						EditorLog.CurrentLogLevel = p.param1;
					}
				}
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