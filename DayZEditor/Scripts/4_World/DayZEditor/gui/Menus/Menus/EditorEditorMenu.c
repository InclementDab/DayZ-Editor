class EditorEditorMenu: EditorMenu
{
	void EditorEditorMenu()
	{
		EditorLog.Trace("EditorEditorMenu");
		
		AddMenuButton(m_Editor.CommandManager[EditorReloadHudCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorReloadBrushesCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorEnvironmentControlCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorPreferencesCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorJoinDiscordCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorOpenWikiCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorDonateCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorHelpCommand]);
	}
}