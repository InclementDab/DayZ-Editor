class EditorImportMenu: EditorMenu
{
	void EditorImportMenu()
	{
		EditorLog.Trace("EditorImportMenu");
		
		AddMenuButton(m_Editor.CommandManager.ImportFromInitFile);
		AddMenuButton(m_Editor.CommandManager.ImportFromExpansionCommand);
		AddMenuButton(m_Editor.CommandManager.ImportFromTerrainBuilderCommand);
		AddMenuButton(m_Editor.CommandManager.ImportFromVPPCommand);
		AddMenuButton(m_Editor.CommandManager.ImportFromCOM);
	}
}