enum TooltipPositions
{
	BOTTOM_LEFT,
	TOP_LEFT,
	BOTTOM_RIGHT,
	TOP_RIGHT
};

class EditorTooltip: ScriptViewTemplate<EditorCommandTooltipController>
{	
	void EditorTooltip(string text, float x, float y)
	{
		m_TemplateController.Name = text;		
		m_TemplateController.NotifyPropertyChanged("Name");
		
		SetPos(x, y);
	}
	
	static EditorTooltip CreateOnButton(string text, Widget button, TooltipPositions position)
	{
		float pos_x, pos_y;
		float b_x, b_y, b_w, b_h;
		button.GetScreenPos(b_x, b_y);
		button.GetScreenSize(b_w, b_h);
		
		EditorTooltip tooltip(text, pos_x, pos_y);
		
		float w, h;
		tooltip.GetLayoutRoot().GetScreenSize(w, h);
		
		switch (position) {
			case TooltipPositions.BOTTOM_LEFT: {
				pos_x = b_x;
				pos_y = b_y + b_h;
				break;
			}		
			
			case TooltipPositions.TOP_LEFT: {
				pos_x = b_x;
				pos_y = b_y - h;
				break;
			}
			
			case TooltipPositions.BOTTOM_RIGHT: {
				pos_x = b_x + b_w;
				pos_y = b_y + b_h;
				break;
			}
			
			case TooltipPositions.TOP_RIGHT: {
				pos_x = b_x + b_w;
				pos_y = b_y - h;
				break;
			}
		}
		
		tooltip.SetPos(pos_x, pos_y);
		return tooltip;
	}
	
	void SetPos(float x, float y)
	{
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