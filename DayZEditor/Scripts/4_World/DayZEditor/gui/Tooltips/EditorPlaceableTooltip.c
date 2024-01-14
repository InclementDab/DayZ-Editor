class EditorPlaceableTooltip: ScriptViewTemplate<EditorPlaceableTooltipController>
{
	protected EditorPlaceableItem m_EditorPlaceableItem;
	protected Object m_Object;
	
	TextWidget Text;
	ItemPreviewWidget Item;
	
	void EditorPlaceableTooltip(notnull EditorPlaceableItem placeable_item, float x, float y)
	{
		m_EditorPlaceableItem = placeable_item;

		if (!IsBlacklistedItem(m_EditorPlaceableItem.GetName())) {
			m_Object = m_EditorPlaceableItem.CreateObject(Vector(0, 0, 0), vector.Zero, 1.0);
			EntityAI entity = EntityAI.Cast(m_Object);
			if (entity && !placeable_item.IsInherited(EditorStaticPlaceableItem)) {
				Item.SetItem(entity);
				Item.SetView(entity.GetViewIndex());
			}
		}
			
		Text.SetText(placeable_item.GetName());
		
		int sx, sy;
		GetScreenSize(sx, sy);
		
		// bounds Y height to screen size
		// 150 is static height of tooltip
		if (y + 300 > sy) {
			y = sy - 300;
		}
		
		m_LayoutRoot.SetPos(x, y);
	}
	
	void ~EditorPlaceableTooltip()
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