class EditorEntityTooltip: ScriptView
{	
	ItemPreviewWidget Item;
	PlayerPreviewWidget Player;
	
	void EditorEntityTooltip(notnull EntityAI entity)
	{
		DayZPlayer player = DayZPlayer.Cast(entity);
		if (player) {
			Player.SetPlayer(player);
		} else {
			Item.SetItem(entity);
			Item.SetView(entity.GetViewIndex());
		}
	}
			
	override string GetLayoutFile() 
	{
		return "Editor\\GUI\\layouts\\ObjectTooltip.layout";
	}
}