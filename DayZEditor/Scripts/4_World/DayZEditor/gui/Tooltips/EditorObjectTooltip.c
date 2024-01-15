class EditorObjectTooltip: ScriptViewTemplate<EditorPlaceableTooltipController>
{
	protected EditorPlaceableItem m_EditorPlaceableItem;
	protected Object m_Object;
	
	TextWidget Text;
	ItemPreviewWidget Item;
	
	void EditorObjectTooltip(notnull Object object, float x, float y)
	{
		m_Object = object;

		Text.SetText(m_Object.GetType());
		
		EntityAI entity = EntityAI.Cast(m_Object);
		if (entity) {
			Item.SetItem(entity);
			Item.SetView(entity.GetViewIndex());
		}
		
		int sx, sy;
		GetScreenSize(sx, sy);
		
		// bounds Y height to screen size
		// 150 is static height of tooltip
		if (y + 300 > sy) {
			y = sy - 300;
		}
		
		m_LayoutRoot.SetPos(x, y);
	}
	
	void ~EditorObjectTooltip()
	{
		GetGame().ObjectDelete(m_Object);
	}
	
	bool IsBlacklistedItem(string item_type)
	{
		array<string> blacklist = { "DZ_LightAI", "Man", "Car" };
		foreach (string blacklist_check: blacklist) {
			if (GetGame().IsKindOf(item_type, blacklist_check)) {
				return true;
			}
		}
		
		return false;
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/layouts/tooltips/EditorTooltip.layout";
	}
}