

typedef ref array<ref EditorMenuItem> EditorMenuItemList;

class EditorMenu: EditorScriptView
{
	protected ref EditorMenuItemList m_MenuItems;
	protected WrapSpacerWidget EditorMenuContent;
	
	void EditorMenu(Widget parent = null) 
	{
		EditorLog.Trace("EditorMenu");
		m_MenuItems = new EditorMenuItemList();
		
		EditorUIManager.CurrentMenu = this;
	}
		
	void ~EditorMenu() 
	{
		EditorLog.Trace("~EditorMenu");
		delete m_MenuItems;
	}
			
	void AddMenuDivider()
	{	
		EditorMenuItemDivider divider();
		AddMenuItem(divider);
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
	void EditorFileMenu(Widget parent = null) 
	{
		EditorLog.Trace("EditorFileMenu::Init");
		
		/*AddMenuButton(EditorOpenCommand);
		AddMenuButton(EditorSaveCommand);
		AddMenuButton(EditorSaveAsCommand);
		AddMenuButton(EditorCloseCommand);
		AddMenuDivider();
		AddMenuButton(EditorExitCommand);*/
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

