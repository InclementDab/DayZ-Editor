
static const float MARKER_ALPHA_ON_SHOW = 1;
static const float MARKER_ALPHA_ON_HIDE = 0.25;

static const int DRAG_BOX_THRESHOLD = 15;
static const int DRAG_BOX_THICKNESS = 2;
static const int DRAG_BOX_COLOR = ARGB(100, 255, 0, 0);


static DayZPlayer CreateDefaultCharacter(vector position = "0 0 0")
{
	DayZPlayer player;
	if (GetWorkbenchGame().GetPlayer() != null) {
		player = GetWorkbenchGame().GetPlayer();
	} 
	
	else if (Class.CastTo(player, GetWorkbenchGame().CreatePlayer(NULL, GetWorkbenchGame().CreateRandomPlayer(), position, 0, "NONE"))) {
		player.GetInventory().CreateInInventory("AviatorGlasses");
    	player.GetInventory().CreateInInventory("AliceBag_Black");
	}

    return player;
}