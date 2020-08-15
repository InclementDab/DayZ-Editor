
static PlayerBase CreateDefaultCharacter(vector position = "0 0 0")
{
	PlayerBase player;
	if (GetGame().GetPlayer() != null) {
		player = GetGame().GetPlayer();
	} else {	
	    player = GetGame().CreatePlayer(NULL, GetGame().CreateRandomPlayer(), position, 0, "NONE");
	    player.GetInventory().CreateInInventory("AviatorGlasses");
	    player.GetInventory().CreateInInventory("AliceBag_Black");
	}
	
    return player;
}


static void ClearWidgetChildren(Widget w)
{
	
	Widget child = w.GetChildren();
	while (child != null) {
		w.RemoveChild(child);
		child = child.GetSibling();
	}
	
}