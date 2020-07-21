static float VIEW_DISTANCE = 5000;
static float OBJECT_VIEW_DISTANCE = 3000;

static bool MAGNET_PLACEMENT = false;

class EditorSettings 
{
	
	static void Load()
	{
		GetGame().GetWorld().SetViewDistance(VIEW_DISTANCE);
		GetGame().GetWorld().SetObjectViewDistance(OBJECT_VIEW_DISTANCE);
	}
	
	static void Save()
	{
		
	}
	
}