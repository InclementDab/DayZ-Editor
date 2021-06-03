class EditorUtilitiesMenu: EditorMenu
{
	void EditorUtilitiesMenu()
	{
		EditorLog.Trace("EditorUtilitiesMenu");
		
		AddMenuButton(m_Editor.CommandManager[EditorExportLoaderCache]);
		AddMenuButton(m_Editor.CommandManager[EditorCheckDuplicatesCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorFindAndReplaceCommand]);
	}
}