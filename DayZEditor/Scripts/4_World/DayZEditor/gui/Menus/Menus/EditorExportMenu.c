class EditorExportMenu: EditorMenu
{
	void EditorExportMenu()
	{
		EditorLog.Trace("EditorExportMenu");
		
		AddMenuButton(m_Editor.CommandManager[EditorExportToInitFile]);
		AddMenuButton(m_Editor.CommandManager[EditorExportToExpansion]);
		AddMenuButton(m_Editor.CommandManager[EditorExportToTerrainBuilder]);
		AddMenuButton(m_Editor.CommandManager[EditorExportToVPP]);
		AddMenuButton(m_Editor.CommandManager[EditorExportToCOM]);
		AddMenuButton(m_Editor.CommandManager[EditorExportToEvents]);
		AddMenuButton(m_Editor.CommandManager[EditorExportToMapGroupPos]);
		AddMenuButton(m_Editor.CommandManager[EditorExportToDZEBin]);
	}
}