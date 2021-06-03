class EditorSettings: Controller
{	
	float ViewDistance 				= 8000;
	float ObjectViewDistance 		= 1500;
	float MarkerViewDistance 		= 1000;
		
	int AutoSaveTimer 				= 240;

	bool LockCameraDuringDialogs 	= true;
	bool ShowBoundingBoxes 			= true;
	bool BrushedObjectMarkers 		= false;
	bool DebugMode 					= false;
	
	string EditorProtoFile 			= "$profile:/Editor/MapGroupProto.xml";
	string EditorBrushFile 			= "$profile:/Editor/EditorBrushes.xml";
	
	LogLevel SelectedLogLevel 		= LogLevel.INFO;
	
	void Load()
	{		
		ViewDistance = GetProfileFloat("ViewDistance", ViewDistance);
		ObjectViewDistance = GetProfileFloat("ObjectViewDistance", ObjectViewDistance);
		MarkerViewDistance = GetProfileFloat("MarkerViewDistance", MarkerViewDistance);
		AutoSaveTimer = GetProfileInt("AutoSaveTimer", AutoSaveTimer);
		
		LockCameraDuringDialogs = GetProfileBool("LockCameraDuringDialogs", LockCameraDuringDialogs);
		ShowBoundingBoxes = GetProfileBool("ShowBoundingBoxes", ShowBoundingBoxes);
		BrushedObjectMarkers = GetProfileBool("BrushedObjectMarkers", BrushedObjectMarkers);
		DebugMode = GetProfileBool("DebugMode", DebugMode);
		
		EditorProtoFile = GetProfileString("EditorProtoFile", EditorProtoFile);
		EditorBrushFile = GetProfileString("EditorBrushFile", EditorBrushFile);
		
		SelectedLogLevel = GetProfileInt("SelectedLogLevel", SelectedLogLevel);
		NotifyPropertyChanged();
	}
	
	void Save()
	{
		SetProfileInt("ViewDistance", ViewDistance);
		SetProfileInt("ObjectViewDistance", ObjectViewDistance);
		SetProfileInt("MarkerViewDistance", MarkerViewDistance);
		SetProfileInt("AutoSaveTimer", AutoSaveTimer);
		
		SetProfileBool("LockCameraDuringDialogs", LockCameraDuringDialogs);
		SetProfileBool("ShowBoundingBoxes", ShowBoundingBoxes);
		SetProfileBool("BrushedObjectMarkers", BrushedObjectMarkers);
		SetProfileBool("DebugMode", DebugMode);
		
		SetProfileString("EditorProtoFile", EditorProtoFile);
		SetProfileString("EditorBrushFile", EditorBrushFile);
		
		SetProfileInt("SelectedLogLevel", SelectedLogLevel);
		
		GetGame().SaveProfile();
	}
	
	static void SetProfileBool(string variable, bool value)
	{
		GetGame().SetProfileString(variable, value.ToString());	
	}
	
	static void SetProfileInt(string variable, int value)
	{
		GetGame().SetProfileString(variable, value.ToString());
	}
	
	static void SetProfileFloat(string variable, float value)
	{
		GetGame().SetProfileString(variable, value.ToString());
	}
	
	static void SetProfileString(string variable, string value)
	{
		GetGame().SetProfileString(variable, value);
	}
	
	static bool GetProfileBool(string variable, bool default = false)
	{
		string value;
		if (GetGame().GetProfileString(variable, value)) {
			return (value == "true" || value == "1");
		}
		
		return default;
	}
	
	static float GetProfileFloat(string variable, float default = 0)
	{
		string value;
		if (GetGame().GetProfileString(variable, value)) {
			return value.ToFloat();
		}
		
		return default;
	}
	
	static int GetProfileInt(string variable, int default = 0)
	{
		string value;
		if (GetGame().GetProfileString(variable, value)) {
			return value.ToInt();
		}
		
		return default;
	}
	
	static string GetProfileString(string variable, string default = "")
	{
		string value;
		if (GetGame().GetProfileString(variable, value)) {
			return value;
		}
		
		return default;
	}
	
	override void PropertyChanged(string property_name)
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
			
			case "DebugMode": {
				if (DebugMode) { 
					GetEditor().GetEditorHud().GetTemplateController().CurrentLogLevel = LogLevel.TRACE;
				} else {
					GetEditor().GetEditorHud().GetTemplateController().CurrentLogLevel = LogLevel.DEBUG;
				}
			}
		}
	}	
}