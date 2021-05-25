class EditorEditorMenu: EditorMenu
{
	void EditorEditorMenu()
	{
		EditorLog.Trace("EditorEditorMenu");
		
		AddMenuButton(m_Editor.CommandManager[EditorReloadHudCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorReloadBrushesCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorExportLoaderCache]);
		AddMenuButton(m_Editor.CommandManager[EditorCheckDuplicatesCommand]);
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