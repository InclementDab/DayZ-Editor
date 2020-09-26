

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

	void AddMenuButton(EditorCommand editor_command)
	{
		ref EditorMenuItemButton menu_item = new EditorMenuItemButton();
		menu_item.SetCommand(editor_command);
		
		ViewBinding view_binding;
		if (!menu_item) return;
		
		menu_item.GetLayoutRoot().FindAnyWidget("EditorMenuItemButton").GetScript(view_binding);
		if (view_binding && editor_command) {
			view_binding.SetRelayCommand(editor_command);
		}
		
		
		AddMenuItem(menu_item);
	}

	void AddMenuItem(ref EditorMenuItem menu_item)
	{		
		m_EditorMenuController.MenuItems.Insert(menu_item);
	}
		
	void RemoveMenuItem(ref EditorMenuItem menu_item)
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
		EditorLog.Trace("EditorFileMenu::Init");
		
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
				
		/*AddMenuButton(EditorUndoCommand);
		AddMenuButton(EditorRedoCommand);
		AddMenuDivider();
		AddMenuButton(EditorCutCommand);
		AddMenuButton(EditorCopyCommand);
		AddMenuButton(EditorPasteCommand);
		AddMenuDivider();
		AddMenuButton(EditorEnvironmentControlCommand);
		AddMenuButton(EditorPreferencesCommand);
		AddMenuDivider();*/
	}
}

class EditorViewMenu: EditorMenu
{	
	void EditorViewMenu(Widget parent = null) 
	{
		EditorLog.Trace("EditorViewMenu");
				
		/*AddMenuButton(EditorCameraControlsCommand);
		AddMenuButton(EditorReloadHudCommand);*/
	}
}

class EditorContextMenu: EditorMenu
{
	void EditorContextMenu(Widget parent = null) 
	{
		EditorLog.Trace("EditorContextMenu");
		/*AddMenuButton(EditorCutCommand);
		AddMenuButton(EditorCopyCommand);
		AddMenuButton(EditorPasteCommand);
		AddMenuDivider();
		AddMenuButton(EditorUndoCommand);
		AddMenuButton(EditorRedoCommand);
		AddMenuDivider();
		AddMenuButton(EditorDeleteCommand);
		AddMenuDivider();
		AddMenuButton(EditorObjectPropertiesCommand);*/
	}
}

class EditorPlaceableContextMenu: EditorMenu
{
	
	void EditorPlaceableContextMenu(Widget parent = null) 
	{
		//AddMenuButton(EditorLootEditorCommand);
	}
}

