class EditorEntityTooltip: ScriptView
{	
	protected EntityAI m_Entity;
	
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
	
	void ~EditorEntityTooltip()
	{
		GetGame().ObjectDelete(m_Entity);
	}
			
	override string GetLayoutFile() 
	{
		return "Editor\\GUI\\layouts\\ObjectTooltip.layout";
	}
}