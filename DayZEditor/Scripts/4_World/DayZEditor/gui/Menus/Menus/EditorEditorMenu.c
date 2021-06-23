class EditorEditorMenu: EditorMenu
{
	void EditorEditorMenu()
	{
		EditorLog.Trace("EditorEditorMenu");
		
		AddMenuButton(m_Editor.CommandManager[EditorReloadCacheCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorReloadBrushesCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorExportLoaderCache]);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorEnvironmentControlCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorPreferencesCommand]);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorJoinDiscordCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorOpenWikiCommand]);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorDonateCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorHelpCommand]);
	}
}