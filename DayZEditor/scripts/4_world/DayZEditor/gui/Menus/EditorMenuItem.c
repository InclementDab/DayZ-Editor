
class EditorMenuItemController: Controller
{
	string LabelText;
	string IconPath;
	string ShortcutText;
	
	// todo use on category menu
	ref EditorMenu ChildMenu;
}

class EditorMenuItem: MVCLayout
{
	protected ref EditorMenuItemController m_EditorMenuItemController;
	protected ImageWidget EditorMenuItemIcon;
			
	void EditorMenuItem(Widget parent = null) {
		EditorLog.Trace("EditorMenuItem");
	}	
	
	void ~EditorMenuItem() {
		EditorLog.Trace("~EditorMenuItem");
	}
	
	override void Init() {
		m_LayoutRoot.Show(true);
		m_EditorMenuItemController = EditorMenuItemController.Cast(GetController());
	}
		
	override typename GetControllerType() {
		return EditorMenuItemController;
	}
}

class EditorMenuItemButton: EditorMenuItem
{	
	protected ref EditorCommand m_EditorCommand;
	
	void SetCommand(EditorCommand editor_command)
	{
		m_EditorCommand = editor_command;
		
		EditorMenuItemIcon.Show(m_EditorCommand.GetIcon() != string.Empty);
		
		m_EditorMenuItemController.LabelText = m_EditorCommand.GetName();
		m_EditorMenuItemController.NotifyPropertyChanged("LabelText");
		
		m_EditorMenuItemController.IconPath = m_EditorCommand.GetIcon();
		m_EditorMenuItemController.NotifyPropertyChanged("IconPath");
		
		m_EditorMenuItemController.ShortcutText = m_EditorCommand.GetKeys();
		m_EditorMenuItemController.NotifyPropertyChanged("ShortcutText");
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
