class EditorMenuItem: ScriptViewTemplate<EditorMenuItemController>
{
	protected ImageWidget EditorMenuItemIcon;
	protected ButtonWidget EditorMenuItemButton;
	
	override string GetLayoutFile() 
	{
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
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/menus/EditorMenuItemDivider.layout";
	}
}

class EditorMenuItemCategory: EditorMenuItem
{
	protected ImageWidget EditorMenuItemCategoryIcon;
	
	void EditorMenuItemCategory(string label, EditorMenu child_menu, EditorCommand editor_command = null)
	{
		EditorMenuItemCategoryIcon.Show(true);
		
		// Icon
		if (editor_command) {
			EditorMenuItemIcon.Show(editor_command.GetIcon() != string.Empty);
			m_TemplateController.IconPath = editor_command.GetIcon();
			m_TemplateController.NotifyPropertyChanged("IconPath");
		}
		
		m_TemplateController.LabelText = label;
		m_TemplateController.NotifyPropertyChanged("LabelText");
		
		m_TemplateController.ChildMenu = child_menu;
		m_TemplateController.NotifyPropertyChanged("ChildMenu");
		
		m_TemplateController.ChildMenu.GetLayoutRoot().Show(false);
		
		ViewBinding view_binding = m_TemplateController.GetViewBinding(EditorMenuItemButton);
		if (view_binding && editor_command) {
			view_binding.SetRelayCommand(editor_command);
		}
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		// Dont show the child menu when the command execute is false
		EditorCommand editor_command = EditorCommand.Cast(m_TemplateController.GetViewBinding(EditorMenuItemButton).GetRelayCommand());
		if (editor_command && !editor_command.CanExecute()) {
			return super.OnMouseEnter(w, x, y);
		}
		
		// Setting this here because menu root needs to be moved before we know where we are
		float x1, y1;
		float sx1, sy1;
		m_LayoutRoot.GetScreenPos(x1, y1);
		m_LayoutRoot.GetScreenSize(sx1, sy1);
		m_TemplateController.ChildMenu.GetLayoutRoot().SetPos(x1 + sx1, y1);
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

class EditorOpenRecentListItem: EditorMenuItem
{	
	// todo: EditorFile struct like in EditorSaveFile
	void EditorOpenRecentListItem(string file_path)
	{		
		m_TemplateController.LabelText = file_path;
		m_TemplateController.NotifyPropertyChanged("LabelText");
		
		ViewBinding view_binding = m_TemplateController.GetViewBinding(EditorMenuItemButton);
		if (view_binding) {
			view_binding.Relay_Command = "OnExecute";
		}
	}
	
	void OnExecute(ButtonCommandArgs args)
	{
		EditorOpenCommand open_command = EditorOpenCommand.Cast(GetEditor().CommandManager[EditorOpenCommand]);
		open_command.OpenFile(m_TemplateController.LabelText);
	}
}

class EditorPreferencesListItem: EditorMenuItem
{	
	void EditorPreferencesListItem(string setting)
	{		
		m_TemplateController.LabelText = setting;
		m_TemplateController.NotifyPropertyChanged("LabelText");
		
		ViewBinding view_binding = m_TemplateController.GetViewBinding(EditorMenuItemButton);
		if (view_binding) {
			view_binding.Relay_Command = "OnExecute";
		}
	}
	
	void OnExecute(ButtonCommandArgs args)
	{
		delete EditorHud.CurrentMenu;
		EditorPreferencesCommand preferences_command = EditorPreferencesCommand.Cast(GetEditor().CommandManager[EditorPreferencesCommand]);
		preferences_command.OpenPreferences(m_TemplateController.LabelText);
	}
}

