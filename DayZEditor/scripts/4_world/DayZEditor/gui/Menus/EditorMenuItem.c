
class EditorMenuItemController: Controller
{
	string LabelText;
	string IconPath;
	string ShortcutText;
	
	// todo use on category menu
	ref EditorMenu ChildMenu;
}

class EditorMenuItem: ScriptView
{
	protected ref EditorMenuItemController m_EditorMenuItemController;
	protected ImageWidget EditorMenuItemIcon;
	protected ButtonWidget EditorMenuItemButton;
			
	void EditorMenuItem(Widget parent = null) 
	{
		EditorLog.Trace("EditorMenuItem");
		
		m_EditorMenuItemController = EditorMenuItemController.Cast(GetController());
	}	
}

class EditorMenuItemCommand: EditorMenuItem
{	
	
	void SetCommand(ref EditorCommand editor_command)
	{
		EditorMenuItemIcon.Show(editor_command.GetIcon() != string.Empty);
		m_EditorMenuItemController = EditorMenuItemController.Cast(GetController());
		Print(m_EditorMenuItemController);
		Print(editor_command);
		m_EditorMenuItemController.LabelText = editor_command.GetName();
		m_EditorMenuItemController.NotifyPropertyChanged("LabelText");
		
		m_EditorMenuItemController.IconPath = editor_command.GetIcon();
		m_EditorMenuItemController.NotifyPropertyChanged("IconPath");
		
		m_EditorMenuItemController.ShortcutText = editor_command.GetKeyDisplay();
		m_EditorMenuItemController.NotifyPropertyChanged("ShortcutText");
		
		ViewBinding view_binding = m_EditorMenuItemController.GetViewBinding(EditorMenuItemButton);
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
