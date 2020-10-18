class EditorSettings: Controller
{	
	float ViewDistance;
	float ObjectViewDistance;
	float MarkerViewDistance;
		
	// Autosave timer in seconds
	int AutoSaveTimer;

	bool LockCameraDuringDialogs;
	bool ShowBoundingBoxes;
	bool DebugMode;
	

	[NonSerialized()]
	ref DropdownListPrefabItem SelectedLogLevel;
	
	void EditorSettings() 
	{
		EditorLog.Trace("EditorSettings");
		ViewDistance = 8000;
		ObjectViewDistance = 1500;
		MarkerViewDistance = 1500;
		AutoSaveTimer = 240;
		LockCameraDuringDialogs = true;
		ShowBoundingBoxes = true;
		DebugMode = false;
	}
	
	static EditorSettings Load(string filename)
	{
		EditorLog.Trace("EditorSettings::Load");
		EditorSettings settings = new EditorSettings();
		if (!FileExist(filename)) {
			Save(settings, filename);
			return settings;
		}
		
		EditorLog.Info("Loading EditorSettings from %1", filename);
		JsonFileLoader<EditorSettings>.JsonLoadFile(filename, settings);		
		return settings;
	}
	
	
	static void Save(EditorSettings settings, string filename)
	{
		EditorLog.Trace("EditorSettings::Save");
		
		EditorLog.Info("Saving EditorSettings to %1", filename);
		JsonFileLoader<EditorSettings>.JsonSaveFile(filename, settings);
	}
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
						
			case "SelectedLogLevel": {
				
				if (SelectedLogLevel.GetTemplateController() && SelectedLogLevel.GetTemplateController().UserData) {
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