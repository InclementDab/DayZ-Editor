class EditorFileMenu: EditorMenu
{
	void EditorFileMenu()
	{
		EditorLog.Trace("EditorFileMenu");
		
		AddMenuButton(m_Editor.CommandManager[EditorNewCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorOpenCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorSaveCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorSaveAsCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorCloseCommand]);
		AddMenuCategory("Import", EditorImportMenu);
		AddMenuCategory("Export", EditorExportMenu);
		AddMenuButton(m_Editor.CommandManager[EditorExitCommand]);
	}
}