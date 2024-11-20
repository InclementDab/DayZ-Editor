class EditorEditorMenu: EditorMenu
{
	void EditorEditorMenu()
	{
		AddMenuButton(m_Editor.CommandManager[EditorReloadBrushesCommand]);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorEnvironmentControlCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorOptionsCommand]);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorJoinDiscordCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorOpenWikiCommand]);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorChangelogCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorDonateCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorHelpCommand]);
	}
}