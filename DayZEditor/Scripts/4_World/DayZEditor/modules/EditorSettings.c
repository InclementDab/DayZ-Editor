class EditorSettings: Controller
{	
	
	float ViewDistance;
	float ObjectViewDistance;
		
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
		AutoSaveTimer = 240;
		LockCameraDuringDialogs = true;
		ShowBoundingBoxes = true;
		DebugMode = false;
	}
	
	static void Load(out EditorSettings settings, string filename)
	{
		EditorLog.Trace("EditorSettings::Load");
		
		// Generate Initial File
		if (!FileExist(filename)) {
			Save(settings, filename);
			return;
		}
		
		EditorLog.Info("Loading EditorSettings from %1", filename);
		// Why the fuck doesnt this load when i RELOAD the editor?!?!?!??!?!?!?!?!!
		// B R U H
		JsonFileLoader<EditorSettings>.JsonLoadFile(filename, settings);
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