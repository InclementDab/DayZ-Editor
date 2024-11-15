class EditorMenu: ScriptView
{			
	protected Editor m_Editor;
	protected EditorHud m_EditorHud;
	protected EditorMenuController m_TemplateController;
	
	void EditorMenu()
	{		
		m_Editor = GetEditor();
		if (m_Editor) {
			m_EditorHud = m_Editor.GetEditorHud();	
		}
		
		m_TemplateController = EditorMenuController.Cast(GetController());
	}
	
	void AddMenuCategory(string label, typename child_menu, Symbols icon, EditorCommand editor_command = null)
	{
		if (child_menu.IsInherited(EditorMenu)) {
			AddMenuCategory(label, EditorMenu.Cast(child_menu.Spawn()), icon, editor_command);
		}
	}
	
	void AddMenuCategory(string label, EditorMenu child_menu, Symbols icon, EditorCommand editor_command = null)
	{
		AddMenuItem(new EditorMenuItemCategory(this, editor_command, label, child_menu, icon));
	}
			
	void AddMenuDivider()
	{	
		AddMenuItem(new EditorMenuItemDivider(this));
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
		AddMenuItem(new EditorMenuItem(this, editor_command));
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
	
	EditorMenuController GetTemplateController()
	{
		return m_TemplateController;
	}
	
	override typename GetControllerType()
	{
		return EditorMenuController;
	}
			
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/menus/EditorMenu.layout";
	}
}