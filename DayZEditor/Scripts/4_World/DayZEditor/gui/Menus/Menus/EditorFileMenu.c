class EditorFileMenu: EditorMenu
{
	void EditorFileMenu()
	{
		EditorLog.Trace("EditorFileMenu");
		
		AddMenuButton(m_Editor.CommandManager[EditorNewCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorOpenCommand]);
		AddMenuCategory("#STR_EDITOR_OPEN_RECENT", EditorOpenRecentMenu, Symbols.FOLDER_GRID, m_Editor.CommandManager[EditorOpenRecentCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorSaveCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorSaveAsCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorCloseCommand]);
		AddMenuDivider();
		AddMenuCategory("#STR_EDITOR_IMPORT", EditorImportMenu, Symbols.ARROW_RIGHT_TO_BRACKET);
		AddMenuCategory("#STR_EDITOR_EXPORT", EditorExportMenu, Symbols.ARROW_RIGHT_FROM_BRACKET);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorExitCommand]);
	}
}