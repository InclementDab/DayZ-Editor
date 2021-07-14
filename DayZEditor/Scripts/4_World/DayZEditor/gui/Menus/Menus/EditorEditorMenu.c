class EditorEditorMenu: EditorMenu
{
	void EditorEditorMenu()
	{
		EditorLog.Trace("EditorEditorMenu");
		
		AddMenuButton(m_Editor.CommandManager[EditorReloadBrushesCommand]);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorEnvironmentControlCommand]);
		AddMenuCategory("#STR_EDITOR_PREFERENCES", EditorPreferencesMenu, m_Editor.CommandManager[EditorPreferencesCommand]);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorJoinDiscordCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorOpenWikiCommand]);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorDonateCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorHelpCommand]);
	}
}