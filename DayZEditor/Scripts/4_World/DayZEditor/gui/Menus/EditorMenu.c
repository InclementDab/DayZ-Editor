class EditorMenu: ScriptViewTemplate<EditorMenuController>
{			
	protected Editor m_Editor;
	protected EditorHud m_EditorHud;
	
	void EditorMenu()
	{
		EditorLog.Trace("EditorMenu");
		
		m_Editor = GetEditor();
		if (m_Editor) {
			m_EditorHud = m_Editor.GetEditorHud();	
		}
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

	void AddMenuButton(typename editor_command_type)
	{
		if (!editor_command_type.IsInherited(EditorCommand)) {
			EditorLog.Error("EditorMenuCommand did not inherit from EditorCommand");
			return;
		}
		
		AddMenuButton(GetEditor().CommandManager[editor_command_type]);
	}
	
	void AddMenuButton(EditorCommand editor_command)
	{
		AddMenuItem(new EditorMenuItemCommand(editor_command));
	}

	void AddMenuItem(EditorMenuItem menu_item)
	{		
		m_TemplateController.MenuItems.Insert(menu_item);
	}
		
	void RemoveMenuItem(EditorMenuItem menu_item)
	{
		m_TemplateController.MenuItems.Remove(m_TemplateController.MenuItems.Find(menu_item));
	}
	
	ObservableCollection<ref EditorMenuItem> GetMenuItems()
	{
		return m_TemplateController.MenuItems;
	}
			
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/menus/EditorMenu.layout";
	}
}