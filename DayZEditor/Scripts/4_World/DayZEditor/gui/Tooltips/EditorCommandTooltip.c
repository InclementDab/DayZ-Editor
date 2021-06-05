class EditorCommandTooltip: ScriptViewTemplate<EditorCommandTooltipController>
{
	void EditorCommandTooltip(EditorCommand editor_command, float x, float y)
	{
		m_TemplateController.Name = editor_command.GetName();
		
		if (editor_command.GetShortcutString() != string.Empty) {
			m_TemplateController.Shortcut = string.Format("(%1)", editor_command.GetShortcutString());
		}
		
		m_TemplateController.NotifyPropertyChanged();
		
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