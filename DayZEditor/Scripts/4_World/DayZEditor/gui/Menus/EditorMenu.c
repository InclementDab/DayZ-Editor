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
	
	void AddMenuCategory(string label, typename child_menu)
	{
		if (child_menu.IsInherited(EditorMenu)) {
			AddMenuCategory(label, EditorMenu.Cast(child_menu.Spawn()));
		}
	}
	
	void AddMenuCategory(string label, EditorMenu child_menu)
	{
		AddMenuItem(new EditorMenuItemCategory(label, child_menu));
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
			
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/menus/EditorMenu.layout";
	}
	
	override typename GetControllerType() 
	{
		return EditorMenuController;
	}
}