


class EditorSettings 
{
	static float VIEW_DISTANCE = 5000;
	static float OBJECT_VIEW_DISTANCE = 3000;
	
	static bool MAGNET_PLACEMENT = false;
	static bool MAINTAIN_HEIGHT = false;
	static bool SIM_CITY_MODE = false;
	
	static string BBOX_COLOR = "#(argb,8,8,3)color(1,1,0,1,co)";
	
	static void Load()
	{
		GetGame().GetWorld().SetViewDistance(VIEW_DISTANCE);
		GetGame().GetWorld().SetObjectViewDistance(OBJECT_VIEW_DISTANCE);
		
		// todo load em from ini file
	}
	
	
	static void Save()
	{
		// todo save settings to ini or something
	}
	
}