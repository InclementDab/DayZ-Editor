class EditorObjectTooltip: ScriptView
{
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
	
	override string GetLayoutFile() 
	{
		return "Editor/gui/layouts/tooltips/EditorTooltip.layout";
	}
}