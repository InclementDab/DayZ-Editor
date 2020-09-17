
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
	
	void AddMenuButton(typename editor_command)
	{
		if (!editor_command.IsInherited(EditorCommand)) {
			EditorLog.Error("Invalid EditorCommand");
			return;
		}
		
		AddMenuButton(editor_command.Spawn());
	}
	
	void AddMenuButton(EditorCommand editor_command)
	{
		EditorMenuItemButton menu_item = new EditorMenuItemButton();
		menu_item.SetCommand(editor_command);
		
		ViewBinding view_binding;
		menu_item.GetLayoutRoot().FindAnyWidget("EditorMenuItemButton").GetScript(view_binding);
		if (view_binding && editor_command) {
			view_binding.SetRelayCommand(editor_command);
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
		
		AddMenuButton(EditorNewCommand);
		AddMenuButton(EditorOpenCommand);
		AddMenuButton(EditorSaveCommand);
		AddMenuButton(EditorSaveAsCommand);
		AddMenuButton(EditorCloseCommand);
		AddMenuDivider();
		AddMenuButton(EditorExitCommand);
	}
}

class EditorEditMenu: EditorMenu
{
	void EditorEditMenu()
	{
		EditorLog.Trace("EditorEditMenu");
				
		AddMenuButton(EditorUndoCommand);
		AddMenuButton(EditorRedoCommand);
		AddMenuDivider();
		AddMenuButton(EditorCutCommand);
		AddMenuButton(EditorCopyCommand);
		AddMenuButton(EditorPasteCommand);
		AddMenuDivider();
		AddMenuButton(EditorEnvironmentControlCommand);
		AddMenuButton(EditorPreferencesCommand);
		AddMenuDivider();
	}
}

class EditorViewMenu: EditorMenu
{
	void EditorViewMenu()
	{
		EditorLog.Trace("EditorViewMenu");
		AddMenuButton(EditorCameraControlsCommand);
		AddMenuButton(EditorReloadHudCommand);
	}
}

class EditorContextMenu: EditorMenu
{
	void EditorContextMenu()
	{
		EditorLog.Trace("EditorContextMenu");
		AddMenuButton(EditorCutCommand);
		AddMenuButton(EditorCopyCommand);
		AddMenuButton(EditorPasteCommand);
		AddMenuDivider();
		AddMenuButton(EditorUndoCommand);
		AddMenuButton(EditorRedoCommand);
		AddMenuDivider();
		AddMenuButton(EditorDeleteCommand);
		AddMenuDivider();
		AddMenuButton(EditorObjectPropertiesCommand);
	}
}

class EditorPlaceableContextMenu: EditorMenu
{
	void EditorPlaceableContextMenu()
	{
		EditorLog.Trace("EditorPlaceableContextMenu");
		AddMenuButton(EditorLootEditorCommand);
	}
}

