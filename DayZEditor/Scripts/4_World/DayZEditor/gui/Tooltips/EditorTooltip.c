class EditorTooltip: ScriptViewTemplate<EditorCommandTooltipController>
{
	void EditorTooltip(string text, float x, float y)
	{
		m_TemplateController.Name = text;		
		m_TemplateController.NotifyPropertyChanged("Name");
		
		int screen_x, screen_y;
		GetScreenSize(screen_x, screen_y);
		
		float w, h;
		m_LayoutRoot.GetScreenSize(w, h);
		if (x + w > screen_x) {
			x -= x + w - screen_x;	
		}
		
		if (y + h > screen_y) {
			y -= y + h - screen_y;
		}
		
		m_LayoutRoot.SetPos(x, y);
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/layouts/tooltips/EditorCommandTooltip.layout";
	}
}