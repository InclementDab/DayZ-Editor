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
	
	void AddMenuCategory(string label, typename child_menu, EditorCommand editor_command = null)
	{
		if (child_menu.IsInherited(EditorMenu)) {
			AddMenuCategory(label, EditorMenu.Cast(child_menu.Spawn()), editor_command);
		}
	}
	
	void AddMenuCategory(string label, EditorMenu child_menu, EditorCommand editor_command = null)
	{
		AddMenuItem(new EditorMenuItemCategory(label, child_menu, editor_command));
	}
			
	void AddMenuDivider()
	{	
		AddMenuItem(new EditorMenuItemDivider());
	}

	void AddMenuButton(EditorCommand editor_command)
	{
		AddMenuItem(new EditorMenuItemCommand(editor_command));
	}

	void AddMenuItem(EditorMenuItem menu_item)
	{		
		m_EditorMenuController.MenuItems.Insert(menu_item);
	}
		
	void RemoveMenuItem(EditorMenuItem menu_item)
	{
		m_EditorMenuController.MenuItems.Remove(m_EditorMenuController.MenuItems.Find(menu_item));
	}
	
	ObservableCollection<ref EditorMenuItem> GetMenuItems()
	{
		return m_EditorMenuController.MenuItems;
	}
			
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/menus/EditorMenu.layout";
	}
	
	override typename GetControllerType() 
	{
		return EditorMenuController;
	}
}