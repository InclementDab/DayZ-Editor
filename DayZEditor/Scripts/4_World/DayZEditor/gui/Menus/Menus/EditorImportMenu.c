class EditorImportMenu: EditorMenu
{
	void EditorImportMenu()
	{
		EditorLog.Trace("EditorImportMenu");
		
		AddMenuButton(m_Editor.CommandManager[EditorImportFromDZE]);
		AddMenuButton(m_Editor.CommandManager[EditorImportFromInit]);
		AddMenuButton(m_Editor.CommandManager[EditorImportFromExpansion]);
		AddMenuButton(m_Editor.CommandManager[EditorImportFromTerrainBuilder]);
		AddMenuButton(m_Editor.CommandManager[EditorImportFromVPP]);
		AddMenuButton(m_Editor.CommandManager[EditorImportFromCOM]);
		AddMenuButton(m_Editor.CommandManager[EditorImportFromObjectSpawner]);
	}
}