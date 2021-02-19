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
		AddMenuCategory("Import", EditorImportMenu);
		AddMenuCategory("Export", EditorExportMenu);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager.ExitCommand);
	}
}