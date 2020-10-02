

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
	
	void EditorMenu() 
	{
		EditorLog.Trace("EditorMenu");
		m_EditorMenuController = EditorMenuController.Cast(GetController());
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

	void AddMenuButton(ref EditorCommand editor_command)
	{
		EditorMenuItemCommand menu_item = new EditorMenuItemCommand();
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
	void EditorFileMenu()
	{
		EditorLog.Trace("EditorFileMenu");
		
		AddMenuButton(m_Editor.CommandManager.NewCommand);
		AddMenuButton(m_Editor.CommandManager.OpenCommand);
		AddMenuButton(m_Editor.CommandManager.SaveCommand);
		AddMenuButton(m_Editor.CommandManager.SaveAsCommand);
		AddMenuButton(m_Editor.CommandManager.CloseCommand);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager.ExitCommand);
	}
}

class EditorEditMenu: EditorMenu
{
	void EditorEditMenu()
	{
		EditorLog.Trace("EditorEditMenu");
				
		AddMenuButton(m_Editor.CommandManager.UndoCommand);
		AddMenuButton(m_Editor.CommandManager.RedoCommand);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager.CutCommand);
		AddMenuButton(m_Editor.CommandManager.CopyCommand);
		AddMenuButton(m_Editor.CommandManager.PasteCommand);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager.EnvironmentControlCommand);
		AddMenuButton(m_Editor.CommandManager.PreferencesCommand);
		AddMenuDivider();
	}
}

class EditorViewMenu: EditorMenu
{	
	void EditorViewMenu()
	{
		EditorLog.Trace("EditorViewMenu");

		AddMenuButton(m_Editor.CommandManager.CameraControlsCommand);
		AddMenuButton(m_Editor.CommandManager.ReloadHudCommand);
		AddMenuButton(m_Editor.CommandManager.ReloadBrushesCommand);
	}
}

class EditorContextMenu: EditorMenu
{
	void EditorContextMenu() 
	{
		EditorLog.Trace("EditorContextMenu");
		AddMenuButton(m_Editor.CommandManager.CutCommand);
		AddMenuButton(m_Editor.CommandManager.CopyCommand);
		AddMenuButton(m_Editor.CommandManager.PasteCommand);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager.UndoCommand);
		AddMenuButton(m_Editor.CommandManager.RedoCommand);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager.DeleteCommand);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager.ObjectPropertiesCommand);
	}
}

class EditorPlaceableContextMenu: EditorMenu
{
	void EditorPlaceableContextMenu()
	{
		AddMenuButton(m_Editor.CommandManager.LootEditorCommand);
	}
}

