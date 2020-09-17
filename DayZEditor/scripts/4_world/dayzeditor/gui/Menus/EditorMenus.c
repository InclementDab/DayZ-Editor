

class EditorMenu: EditorMVCLayout
{
	private ref array<ref EditorMenuItem> m_MenuItems = {};
	protected WrapSpacerWidget EditorMenuContent;
	
	void EditorMenu() {
		EditorLog.Trace("EditorMenu");
		if (m_LayoutRoot) {
			m_LayoutRoot.Show(false);
		}
	}
	
	void ~EditorMenu() {
		EditorLog.Trace("~EditorMenu");
		delete m_MenuItems;
	}
	
	override void Show()
	{
		EditorLog.Trace("EditorMenu::Show");
		super.Show();
		
		m_LayoutRoot.Show(true);
		m_EditorHud.SetMenu(this);
	}
	
	override void Close()
	{
		EditorLog.Trace("EditorMenu::Close");
		super.Close();
	}
	
	void SetPosition(float x, float y) {
		m_LayoutRoot.SetPos(x, y);
	}
	
	void AddMenuDivider()
	{	
		EditorMenuItemDivider divider();
		AddMenuItem(divider);
	}
	
	void AddMenuButton(string title, string relay_command = "", string shortcut_text = "", string icon = "")
	{
		EditorMenuItemButton menu_item = new EditorMenuItemButton();
		menu_item.SetText(title);
		menu_item.SetIcon(icon);
		menu_item.SetShortcut(shortcut_text);
		
		ViewBinding view_binding;
		menu_item.GetLayoutRoot().FindAnyWidget("EditorMenuItemButton").GetScript(view_binding);
		if (view_binding && relay_command != string.Empty) {
			view_binding.SetRelayCommand(relay_command);
		}
		
		AddMenuItem(menu_item);
	}
	
	void AddMenuItem(ref EditorMenuItem menu_item)
	{
		if (menu_item) {
			EditorMenuContent.AddChild(menu_item.GetLayoutRoot());
			m_MenuItems.Insert(menu_item);
		}
	}
		
	void RemoveMenuItem(ref EditorMenuItem menu_item)
	{
		EditorMenuContent.RemoveChild(menu_item.GetLayoutRoot());
		m_MenuItems.Remove(m_MenuItems.Find(menu_item));
		delete menu_item;
	}
		
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/menus/EditorMenu.layout";
	}
}

class EditorFileMenu: EditorMenu
{
	void EditorFileMenu()
	{
		EditorLog.Trace("EditorFileMenu");
		
		AddMenuButton("New", "EditorToolbarNewCommand", "Ctrl + N");
		AddMenuButton("Open", "EditorToolbarOpenCommand", "Ctrl + O");
		AddMenuButton("Save", "EditorToolbarSaveCommand", "Ctrl + S");
		AddMenuButton("Save As...", "EditorToolbarSaveAsCommand", "Ctrl + Shift + S");
		AddMenuButton("Close", "EditorToolbarCloseCommand", "Ctrl + W");
	}
}

class EditorEditMenu: EditorMenu
{
	void EditorEditMenu()
	{
		EditorLog.Trace("EditorEditMenu");
			
		AddMenuDivider();	
		AddMenuButton("Undo", "EditorToolbarUndoCommand", "Ctrl + Z");
		AddMenuButton("Redo", "EditorToolbarRedoCommand", "Ctrl + Y");
		AddMenuDivider();
		AddMenuButton("Preferences...", "EditorToolbarPreferencesCommand");
	}
}

class EditorViewMenu: EditorMenu
{
	void EditorViewMenu()
	{
		EditorLog.Trace("EditorViewMenu");
		
		AddMenuButton("Camera Controls...", "EditorToolbarCameraControlCommand");
		AddMenuButton("Reload Editor UI", "EditorToolbarReloadUICommand");
		AddMenuDivider();
	}
}


