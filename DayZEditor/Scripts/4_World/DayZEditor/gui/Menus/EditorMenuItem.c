class EditorMenuItem: ScriptViewTemplate<EditorMenuItemController>
{
	protected ImageWidget EditorMenuItemIcon;
	protected ButtonWidget EditorMenuItemButton;
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/menus/EditorMenuItem.layout";
	}
}

class EditorMenuItemCommand: EditorMenuItem
{	
	void EditorMenuItemCommand(EditorCommand editor_command)
	{
		EditorMenuItemIcon.Show(editor_command.GetIcon() != string.Empty);
		
		m_TemplateController.LabelText = editor_command.GetName();
		m_TemplateController.NotifyPropertyChanged("LabelText");
		
		m_TemplateController.IconPath = editor_command.GetIcon();
		m_TemplateController.NotifyPropertyChanged("IconPath");
		
		m_TemplateController.ShortcutText = editor_command.GetShortcutString();
		m_TemplateController.NotifyPropertyChanged("ShortcutText");
		
		ViewBinding view_binding = m_TemplateController.GetViewBinding(EditorMenuItemButton);
		if (view_binding) {
			view_binding.SetRelayCommand(editor_command);
		}
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
	protected ImageWidget EditorMenuItemCategoryIcon;
	
	void EditorMenuItemCategory(string label, EditorMenu child_menu)
	{
		EditorMenuItemCategoryIcon.Show(true);
		
		m_TemplateController.LabelText = label;
		m_TemplateController.NotifyPropertyChanged("LabelText");
		
		m_TemplateController.ChildMenu = child_menu;
		m_TemplateController.NotifyPropertyChanged("ChildMenu");
		
		m_TemplateController.ChildMenu.GetLayoutRoot().Show(false);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		// Setting this here because menu root needs to be moved before we know where we are
		float x1, y1;
		m_LayoutRoot.GetScreenPos(x1, y1);
		m_TemplateController.ChildMenu.GetLayoutRoot().SetPos(x1 + 275, y1); // 275 is menu width
		m_TemplateController.ChildMenu.GetLayoutRoot().Show(true);
		return super.OnMouseEnter(w, x, y);
	}	
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (enterW != m_TemplateController.ChildMenu.GetLayoutRoot().FindAnyWidget(enterW.GetName())) {
			m_TemplateController.ChildMenu.GetLayoutRoot().Show(false);
		}
		return super.OnMouseLeave(w, enterW, x, y);
	}
}
