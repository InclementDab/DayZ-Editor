class EditorFileMenu: EditorMenu
{
	void EditorFileMenu()
	{
		EditorLog.Trace("EditorFileMenu");
		
		AddMenuButton(m_Editor.CommandManager[EditorNewCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorOpenCommand]);
		AddMenuCategory("Open Recent", EditorOpenRecentMenu, m_Editor.CommandManager[EditorOpenRecentCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorSaveCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorSaveAsCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorCloseCommand]);
		AddMenuDivider();
		AddMenuCategory("Import", EditorImportMenu);
		AddMenuCategory("Export", EditorExportMenu);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorExitCommand]);
	}
}