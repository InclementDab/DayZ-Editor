class EditorExportMenu: EditorMenu
{
	void EditorExportMenu()
	{
		EditorLog.Trace("EditorExportMenu");
		
		AddMenuButton(m_Editor.CommandManager.ExportToInitFile);
		AddMenuButton(m_Editor.CommandManager.ExportToExpansion);
		AddMenuButton(m_Editor.CommandManager.ExportToTerrainBuilder);
		AddMenuButton(m_Editor.CommandManager.ExportToVPP);
		AddMenuButton(m_Editor.CommandManager.ExportToCOM);
		AddMenuButton(m_Editor.CommandManager.ExportToEvents);
	}
}