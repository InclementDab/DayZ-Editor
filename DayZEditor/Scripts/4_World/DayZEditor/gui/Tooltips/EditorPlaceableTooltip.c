class EditorPlaceableTooltip: ScriptViewTemplate<EditorPlaceableTooltipController>
{
	protected TextWidget HeaderLabel;
	
	void SetTitle(string title)
	{
		//EditorLog.Trace("EditorTooltip::SetTitle %1", title);
		m_TemplateController.ContentTitle = title;
		m_TemplateController.NotifyPropertyChanged("ContentTitle");
		
		float w, h, lw, lh;
		HeaderLabel.GetScreenSize(w, h);
		m_LayoutRoot.GetScreenSize(lw, lh);
		m_LayoutRoot.SetSize(w + 80, lh);
	}
	
	void SetContent(string text)
	{
		//EditorLog.Trace("EditorTooltip::SetContent %1", text);

		m_TemplateController.ContentText = text;
		m_TemplateController.NotifyPropertyChanged("ContentText");
	}
	
	void SetContent(Object item)
	{
		//EditorLog.Trace("EditorTooltip::SetContent %1", item.ToString());

		m_TemplateController.ContentItemData = item;
		m_TemplateController.NotifyPropertyChanged("ContentItemData");
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