class EditorMenuItem: ScriptViewTemplate<EditorMenuItemController>
{
	protected ImageWidget EditorMenuItemIcon;
	protected ButtonWidget EditorMenuItemButton;
}

class EditorMenuItemCommand: EditorMenuItem
{	
	void SetCommand(ref EditorCommand editor_command)
	{
		EditorMenuItemIcon.Show(editor_command.GetIcon() != string.Empty);
		
		m_TemplateController.LabelText = editor_command.GetName();
		m_TemplateController.NotifyPropertyChanged("LabelText");
		
		m_TemplateController.IconPath = editor_command.GetIcon();
		m_TemplateController.NotifyPropertyChanged("IconPath");
		
		m_TemplateController.ShortcutText = editor_command.GetShortcut();
		m_TemplateController.NotifyPropertyChanged("ShortcutText");
		
		ViewBinding view_binding = m_TemplateController.GetViewBinding(EditorMenuItemButton);
		if (view_binding) {
			view_binding.SetRelayCommand(editor_command);
		}
	}
		
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/menus/EditorMenuItem.layout";
	}
}

class EditorMenuItemDivider: EditorMenuItem
{	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/menus/EditorMenuItemDivider.layout";
	}
}

class EditorMenuItemCategory: EditorMenuItem
{
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/menus/EditorMenuItemCategory.layout";
	}
}
