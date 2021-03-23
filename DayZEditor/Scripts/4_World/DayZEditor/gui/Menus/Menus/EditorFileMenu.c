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
		AddMenuCategory("Import", EditorImportMenu);
		AddMenuCategory("Export", EditorExportMenu);
		AddMenuButton(m_Editor.CommandManager.ExitCommand);
	}
}