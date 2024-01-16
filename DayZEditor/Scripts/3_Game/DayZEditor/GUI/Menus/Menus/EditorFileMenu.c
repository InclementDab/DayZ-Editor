class EditorFileMenu: EditorMenu
{
	void EditorFileMenu()
	{
		EditorLog.Trace("EditorFileMenu");
		
		AddMenuButton(m_Editor.CommandManager[EditorNewCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorOpenCommand]);
		AddMenuCategory("#STR_EDITOR_OPEN_RECENT", EditorOpenRecentMenu, m_Editor.CommandManager[EditorOpenRecentCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorSaveCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorSaveAsCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorCloseCommand]);
		AddMenuDivider();
		AddMenuCategory("#STR_EDITOR_IMPORT", EditorImportMenu);
		AddMenuCategory("#STR_EDITOR_EXPORT", EditorExportMenu);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorExitCommand]);
	}
}