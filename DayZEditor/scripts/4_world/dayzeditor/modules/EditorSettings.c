

class EditorSettings
{
	
	
	// shit thats gotta be changed
	float ViewDistance = 12000;
	float ObjectViewDistance = 5000;
	float MarkerViewDistance = 1500;
		
	// Autosave timer in seconds
	int AutosaveTimer = 240;

	
	void EditorSettings() {
		EditorLog.Trace("EditorSettings");
	}
	
	static EditorSettings Load(string filename)
	{
		// todo load settings from ini or something
		return new EditorSettings();
	}
	
	
	static void Save(EditorSettings settings, string filename)
	{
		// todo save settings to ini or something
	}


}

