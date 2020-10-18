class EditorSettings
{	
	// shit thats gotta be changed
	float ViewDistance = 8000;
	float ObjectViewDistance = 1500;
	float MarkerViewDistance = 1500;
		
	// Autosave timer in seconds
	int AutoSaveTimer = 240;

	bool LockCameraDuringDialogs = true;
	bool ShowBoundingBoxes = true;
	bool DebugMode;
	
	void EditorSettings() 
	{
		EditorLog.Trace("EditorSettings");
	}
	
	static EditorSettings Load(string filename)
	{
		EditorSettings settings = new EditorSettings();
		if (!FileExist(filename)) {
			JsonFileLoader<EditorSettings>.JsonSaveFile(filename, settings);
			return settings;
		}
				
		JsonFileLoader<EditorSettings>.JsonLoadFile(filename, settings);		
		return settings;
	}
	
	
	static void Save(EditorSettings settings, string filename)
	{
		JsonFileLoader<EditorSettings>.JsonSaveFile(filename, settings);
	}

	void Reload()
	{
		GetGame().GetWorld().SetViewDistance(ViewDistance);
		GetGame().GetWorld().SetObjectViewDistance(ObjectViewDistance);
	}
}