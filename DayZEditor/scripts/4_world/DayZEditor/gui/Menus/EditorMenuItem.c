
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
	void SetText(string text) {
		m_EditorMenuItemController.LabelText = text;
		m_EditorMenuItemController.NotifyPropertyChanged("LabelText");
	}
	
	void SetIcon(string icon) {
		m_EditorMenuItemController.IconPath = icon;
		m_EditorMenuItemController.NotifyPropertyChanged("IconPath");
	}
	
	void SetShortcut(string shortcut) {
		m_EditorMenuItemController.ShortcutText = shortcut;
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
