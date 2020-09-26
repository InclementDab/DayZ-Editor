

class EditorMenuController: Controller
{
	ref ObservableCollection<ref EditorMenuItem> MenuItems = new ObservableCollection<ref EditorMenuItem>("MenuItems", this);
	
	void ~EditorMenuController()
	{
		delete MenuItems;
	}
}

class EditorMenu: EditorScriptView
{	
	protected EditorMenuController m_EditorMenuController;	
	
	void EditorMenu(Widget parent = null) 
	{
		EditorLog.Trace("EditorMenu");
		
		m_EditorMenuController = GetController();
		EditorUIManager.CurrentMenu = this;
	}
		
	void ~EditorMenu() 
	{
		EditorLog.Trace("~EditorMenu");
	}
			
	void AddMenuDivider()
	{	
		EditorMenuItemDivider divider();
		AddMenuItem(divider);
	}
	
	void AddMenuButton(typename editor_command)
	{
		if (editor_command.IsInherited(EditorCommand)) {
			AddMenuButton(editor_command.Spawn());
		}
	}

	void AddMenuButton(ref EditorCommand editor_command)
	{
		Print(editor_command);
		EditorMenuItemCommand menu_item = new EditorMenuItemCommand(GetLayoutRoot());
		menu_item.SetCommand(editor_command);
		AddMenuItem(menu_item);
	}

	void AddMenuItem(EditorMenuItem menu_item)
	{		
		m_EditorMenuController.MenuItems.Insert(menu_item);
	}
		
	void RemoveMenuItem(EditorMenuItem menu_item)
	{
		m_EditorMenuController.MenuItems.Remove(m_EditorMenuController.MenuItems.Find(menu_item));
	}
			
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/menus/EditorMenu.layout";
	}
	
	override typename GetControllerType() {
		return EditorMenuController;
	}
}

class EditorFileMenu: EditorMenu
{
	void EditorFileMenu(Widget parent = null) 
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
	void EditorEditMenu(Widget parent = null) 
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
	void EditorViewMenu(Widget parent = null) 
	{
		EditorLog.Trace("EditorViewMenu");
				
		AddMenuButton(EditorCameraControlsCommand);
		AddMenuButton(EditorReloadHudCommand);
	}
}

class EditorContextMenu: EditorMenu
{
	void EditorContextMenu(Widget parent = null) 
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
	
	void EditorPlaceableContextMenu(Widget parent = null) 
	{
		AddMenuButton(EditorLootEditorCommand);
	}
}

