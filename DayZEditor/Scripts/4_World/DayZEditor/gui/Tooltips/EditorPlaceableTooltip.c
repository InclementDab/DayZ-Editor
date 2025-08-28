class EditorPlaceableTooltip: ScriptView
{
	TextWidget HeaderLabel;
	ItemPreviewWidget ContentItem;
	ImageWidget BackupImage;
	protected Object m_MyObject;

	void ~EditorPlaceableTooltip()
	{
		if (m_MyObject) {
			m_MyObject.Delete();
		}
	}
	
	void SetTitle(string title)
	{
		HeaderLabel.SetText(title);
	}

	void SetContent(Object item)
	{
		m_MyObject = item;
		EntityAI entity = EntityAI.Cast(item);
		if (entity) {			
			ContentItem.SetItem(entity);
			BackupImage.Show(false);
		}
	}
	
	void SetPosition(float x, float y)
	{
		int sx, sy;
		GetScreenSize(sx, sy);
		
		// bounds Y height to screen size
		// 150 is static height of tooltip
		if (y + 300 > sy) {
			y = sy - 300;
		}
		
		m_LayoutRoot.SetPos(x, y);
	}

	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/layouts/tooltips/EditorTooltip.layout";
	}
}