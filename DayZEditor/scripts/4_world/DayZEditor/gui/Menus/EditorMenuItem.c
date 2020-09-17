
class EditorMenuItemController: Controller
{
	string LabelText;
	string IconPath;
	string ShortcutText;
	
	// todo use on category menu
	ref EditorMenu ChildMenu;
}

class EditorMenuItem: EditorMVCLayout
{
	protected ref EditorMenuItemController m_EditorMenuItemController;
	
	void EditorMenuItem() {
		EditorLog.Trace("EditorMenuItem");
		if (m_LayoutRoot) {
			m_LayoutRoot.Show(true);
		}
		
		m_EditorMenuItemController = EditorMenuItemController.Cast(GetController());
	}
	
	void ~EditorMenuItem() {
		EditorLog.Trace("~EditorMenuItem");
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
		
		m_EditorMenuItemController.LabelText = m_EditorCommand.GetName();
		m_EditorMenuItemController.IconPath = m_EditorCommand.GetIcon();
		m_EditorMenuItemController.ShortcutText = m_EditorCommand.GetKeys();
		
		m_EditorMenuItemController.NotifyPropertyChanged("LabelText");
		m_EditorMenuItemController.NotifyPropertyChanged("IconPath");
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
