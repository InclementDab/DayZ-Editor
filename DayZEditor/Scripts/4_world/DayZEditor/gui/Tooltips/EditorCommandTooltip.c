
class EditorCommandTooltipController: Controller
{
	string Name;
	string Shortcut;
}

class EditorCommandTooltip: ScriptViewTemplate<EditorCommandTooltipController>
{
	void EditorCommandTooltip(EditorCommand editor_command, float x, float y)
	{
		m_TemplateController.Name = editor_command.GetName();
		
		if (editor_command.GetShortcut() != string.Empty) {
			m_TemplateController.Shortcut = string.Format("(%1)", editor_command.GetShortcut());
		}
		
		m_TemplateController.NotifyPropertyChanged();
		
		m_LayoutRoot.SetPos(x, y);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/layouts/tooltips/EditorCommandTooltip.layout";
	}
}
