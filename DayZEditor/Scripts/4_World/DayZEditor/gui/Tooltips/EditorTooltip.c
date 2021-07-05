class EditorTooltip: ScriptViewTemplate<EditorCommandTooltipController>
{	
	TextWidget EditorCommandText;
	TextWidget EditorCommandShortcut;
	
	void EditorTooltip(string text, float x, float y, string shortcut_text = "")
	{
		m_TemplateController.Name = text;				
		m_TemplateController.Shortcut = shortcut_text;
		m_TemplateController.NotifyPropertyChanged();
		
		int text_x, text_y, short_x, short_y;
		EditorCommandText.GetTextSize(text_x, text_y);
		EditorCommandShortcut.GetTextSize(short_x, short_y);

		GetLayoutRoot().SetSize(text_x + short_x + 40, text_y);
		
		SetPos(x, y);
	}
	
	static EditorTooltip CreateOnButton(string text, Widget button, TooltipPositions position, string shortcut_text = "")
	{
		float pos_x, pos_y;
		float b_x, b_y, b_w, b_h;
		button.GetScreenPos(b_x, b_y);
		button.GetScreenSize(b_w, b_h);
		
		EditorTooltip tooltip(text, pos_x, pos_y, shortcut_text);
		
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
	
	static EditorTooltip CreateOnButton(EditorCommand editor_command, Widget button, TooltipPositions position)
	{
		string text = editor_command.GetName();
		string shortcut;
		if (editor_command.GetShortcutString() != string.Empty) {
			shortcut = string.Format("(%1)", editor_command.GetShortcutString());
		}
				
		return EditorTooltip.CreateOnButton(text, button, position, shortcut);
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